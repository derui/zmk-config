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

      // shift
    /* NSS(W, "me"), */
    /* NSS(E, "-"), */
    /* NSS(R, "he"), */
    /* NSS(U, "yu"), */
    /* NSS(I, "wa"), */
    /* NSS(O, "ro"), */
    /* NSS(A, "e"), */
    /* NSS(S, "se"), */
    /* NSS(D, "mi"), */
    /* NSS(F, "ra"), */
    /* NSS(G, "ho"), */
    /* NSS(H, "so"), */
    /* NSS(J, "yo"), */
    /* NSS(K, "tu"), */
    /* NSS(L, "sa"), */
    /* NSS(SCLN, "ke"), */
    /* NSS(Z, "hi"), */
    /* NSS(X, "fu"), */
    /* NSS(M, "ne"), */
    /* NSS(COMM, "ya"), */
    /* NSS(DOT, "mu"), */
    /* NSS(SLSH, "nu"), */

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
