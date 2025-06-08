/*
 * Copyright (c) 2025 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_japanese_input
#define JAPANESE_INPUT_CAPTURE_SIZE 3

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>
#include <dt-bindings/zmk/keys.h>

#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/behavior.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// data model of the japanese input
struct behavior_japanese_input_definition {
  // resultと対応するキーコードのマッピング。releaseされたときに適用される
  uint32_t mapping[JAPANESE_INPUT_CAPTURE_SIZE];

  // 実際に押されるキーコードの一覧。0から順に送出される
  uint32_t result_seq[JAPANESE_INPUT_CAPTURE_SIZE];
};


/*   /\* 各シフトシーケンスを生成するためのdefinition */
/*    次のように使う。 */

/*    NSI(A) => Aの単打 */
/*    NM2(F, K) => FとKの同時シフト */
/*    MN3(F, K, Q) => AとKとQの同時シフト */
/*  *\/ */
/* #define NSI(k, seq) { N_ ## k, seq } */
/* #define NSS(k, seq) { SHIFT_BIT | N_ ## k, seq } */
/* #define NM2(k1, k2, seq) { (N_ ## k1 | (N_ ## k2 << 5)), seq } */
/* #define NM3(k1, k2, k3, seq) { N_ ## k1 | (N_ ## k2 << 5) | (N_ ## k3 << 10), seq} */

/* /\* シフトキーを定義する構造体。 *\/ */
/* typedef struct { */
/*   /\* LSB is shift mark. *\/ */
/*   uint16_t keycodes; */
/*   const char* sequence; */
/* } seq_definition_t; */

/* /\* 複数キーの定義 *\/ */
/* seq_definition_t seq_definitions[] = { */
  
/*   /\* Q行 *\/ */
/*   NSI(W, "ku"), */
/*   NSI(E, "si"), */
/*   NSI(R, "ma"), */
/*   NSI(U, "wo"), */
/*   NSI(I, "ki"), */
/*   NSI(O, "su"), */

/*   /\* A行 *\/ */
/*   NSI(A, "ta"), */
/*   NSI(S, "u"), */
/*   NSI(D, "na"), */
/*   NSI(F, "ka"), */
/*   NSI(G, "te"), */
/*   NSI(H, "ni"), */
/*   NSI(J, "ru"), */
/*   NSI(K, "no"), */
/*   NSI(L, "i"), */
/*   NSI(SCLN, "nn"), */
  
/*   /\* Z行 *\/ */
/*   NSI(Z, "ko"), */
/*   NSI(X, "ha"), */
/*   NSI(C, "mo"), */
/*   NSI(V, "to"), */
/*   NSI(B, "a"), */
/*   NSI(N, "o"), */
/*   NSI(M, "ra"), */
/*   NSI(COMM, "ri"), */
/*   NSI(DOT, "xtu"), */
/*   NSI(SLSH, "tu"), */
struct behavior_japanese_input_definition behavior_japanese_input_definitions[] = {

};

// 押されているkeycodeを保持する配列
uint32_t captured_keycodes[JAPANESE_INPUT_CAPTURE_SIZE] = {0, 0, 0};

// 対象とするキーコードをcaptureする
// captureされたkeycodeは常に0番目に設定され、各々の値は後ろにずらしていく
void japanese_input_capture_keycode(uint32_t keycode) {
    for (int i = 0; i < JAPANESE_INPUT_CAPTURE_SIZE - 1; i++) {
      captured_keycodes[i + 1] = captured_keycodes[i];
    }
    captured_keycodes[0] = keycode;
}

static int on_japanese_input_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    LOG_DBG("position %d keycode 0x%02X", event.position, binding->param1);
    return raise_zmk_keycode_state_changed_from_encoded(binding->param1, true, event.timestamp);
}

static int on_japanese_input_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    LOG_DBG("position %d keycode 0x%02X", event.position, binding->param1);
    return raise_zmk_keycode_state_changed_from_encoded(binding->param1, false, event.timestamp);
}

static const struct behavior_driver_api behavior_japanese_input_driver_api = {
    .binding_pressed = on_japanese_input_binding_pressed,
    .binding_released = on_japanese_input_binding_released,
};

static int japanese_input_init(const struct device *dev) {
  static bool initialized = false;

  if (!initialized) {
    // initialize the captured keycodes to zero
    captured_keycodes = {0,0,0};
  }
  initialized = true;
  return 0;
};

#define KP_INST(n)                                                                                 \
    BEHAVIOR_DT_INST_DEFINE(n, japanese_input_init, NULL, NULL, NULL, POST_KERNEL,                                \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_key_press_driver_api);

DT_INST_FOREACH_STATUS_OKAY(KP_INST)
