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

struct behavior_japanese_input_definition behavior_japanese_input_definitions[] = {
    {.mapping = { A }, .result_seq = {S, H, I}},
    {.mapping = { S }, .result_seq = {N, O}},
    {.mapping = { D }, .result_seq = {K, U}},
    {.mapping = { F }, .result_seq = {R, U}},
    {.mapping = { G }, .result_seq = {N, I}},
    {.mapping = { H }, .result_seq = {M, O}},
    {.mapping = { J }, .result_seq = {K, A}},
    {.mapping = { K }, .result_seq = {N, N}},
    {.mapping = { L }, .result_seq = {N, A}},
    {.mapping = { SCLN }, .result_seq = {U}},
    {.mapping = { Z }, .result_seq = {R, E}},
    {.mapping = { X }, .result_seq = {R, I}},
    {.mapping = { C }, .result_seq = {K, I}},
    {.mapping = { V }, .result_seq = {W, O}},
    {.mapping = { B }, .result_seq = {O}},
    {.mapping = { N }, .result_seq = {A}},
    {.mapping = { M }, .result_seq = {H, A}},
    {.mapping = { COMM }, .result_seq = {T, E}},
    {.mapping = { DOT }, .result_seq = {M, A}},
    {.mapping = { SLSH }, .result_seq = {T, I}},

    // シフトキー
    {.mapping = {SPACE}, .result_seq = {SPACE}},
    {.mapping = {ENTER}, .result_seq = {ENTER}},

    // シフト面
    {.mapping = {SPACE, R}, .result_seq = {H, E}},
    {.mapping = {SPACE, U}, .result_seq = {Y, U}},
    {.mapping = {SPACE, I}, .result_seq = {W, A}},
    {.mapping = {SPACE, O}, .result_seq = {R, O}},
    {.mapping = {SPACE, A}, .result_seq = {E}},
    {.mapping = {SPACE, S}, .result_seq = {S, E}},
    {.mapping = {SPACE, D}, .result_seq = {M, I}},
    {.mapping = {SPACE, F}, .result_seq = {R, A}},
    {.mapping = {SPACE, G}, .result_seq = {H, O}},
    {.mapping = {SPACE, H}, .result_seq = {S, O}},
    {.mapping = {SPACE, J}, .result_seq = {Y, O}},
    {.mapping = {SPACE, K}, .result_seq = {T, U}},
    {.mapping = {SPACE, L}, .result_seq = {S, A}},
    {.mapping = {SPACE, SCLN}, .result_seq = {K, E}},
    {.mapping = {SPACE, Z}, .result_seq = {H, I}},
    {.mapping = {SPACE, X}, .result_seq = {F, U}},
    {.mapping = {SPACE, M}, .result_seq = {N, E}},
    {.mapping = {SPACE, COMM}, .result_seq = {Y, A}},
    {.mapping = {SPACE, DOT}, .result_seq = {M, U}},
    {.mapping = {SPACE, SLSH}, .result_seq = {N, U}},

    {.mapping = {ENTER, R}, .result_seq = {H, E}},
    {.mapping = {ENTER, U}, .result_seq = {Y, U}},
    {.mapping = {ENTER, I}, .result_seq = {W, A}},
    {.mapping = {ENTER, O}, .result_seq = {R, O}},
    {.mapping = {ENTER, A}, .result_seq = {E}},
    {.mapping = {ENTER, S}, .result_seq = {S, E}},
    {.mapping = {ENTER, D}, .result_seq = {M, I}},
    {.mapping = {ENTER, F}, .result_seq = {R, A}},
    {.mapping = {ENTER, G}, .result_seq = {H, O}},
    {.mapping = {ENTER, H}, .result_seq = {S, O}},
    {.mapping = {ENTER, J}, .result_seq = {Y, O}},
    {.mapping = {ENTER, K}, .result_seq = {T, U}},
    {.mapping = {ENTER, L}, .result_seq = {S, A}},
    {.mapping = {ENTER, SCLN}, .result_seq = {K, E}},
    {.mapping = {ENTER, Z}, .result_seq = {H, I}},
    {.mapping = {ENTER, X}, .result_seq = {F, U}},
    {.mapping = {ENTER, M}, .result_seq = {N, E}},
    {.mapping = {ENTER, COMM}, .result_seq = {Y, A}},
    {.mapping = {ENTER, DOT}, .result_seq = {M, U}},
    {.mapping = {ENTER, SLSH}, .result_seq = {N, U}},

    // TODO convert then
    /* ND(W, J, "zu"), */
    /* ND(R, J, "be"), */
    /* ND(U, F, "go"), */
    /* ND(I, F, "do"), */
    /* ND(O, F, "da"), */
    /* ND(A, J, "zi"), */
    /* ND(S, J, "ze"), */
    /* ND(D, J, "gu"), */
    /* ND(G, J, "bo"), */
    /* ND(H, F, "zo"), */
    /* ND(J, F, "ga"), */
    /* ND(K, F, "du"), */
    /* ND(L, F, "za"), */
    /* ND(SCLN, F, "ge"), */
    /* ND(Z, J, "bi"), */
    /* ND(X, J, "bu"), */
    /* ND(C, J, "gi"), */
    /* ND(M, F, "ba"), */
    /* ND(COMM, F, "de"), */
    /* ND(SLSH, F, "di"), */
    /* ND(R, M, "pe"), */
    /* ND(G, M, "po"), */
    /* ND(Z, M, "pi"), */
    /* ND(X, M, "pu"), */
    /* ND(V, M, "pa"), */
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
