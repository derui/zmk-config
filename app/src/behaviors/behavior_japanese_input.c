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

    // 濁音・半濁音
    {.mapping = {W, J}, .result_seq = {Z, U}},
    {.mapping = {R, J}, .result_seq = {B, E}},
    {.mapping = {U, F}, .result_seq = {G, O}},
    {.mapping = {I, F}, .result_seq = {D, O}},
    {.mapping = {O, F}, .result_seq = {D, A}},
    {.mapping = {A, J}, .result_seq = {Z, I}},
    {.mapping = {S, J}, .result_seq = {Z, E}},
    {.mapping = {D, J}, .result_seq = {G, U}},
    {.mapping = {G, J}, .result_seq = {B, O}},
    {.mapping = {H, F}, .result_seq = {Z, O}},
    {.mapping = {J, F}, .result_seq = {G, A}},
    {.mapping = {K, F}, .result_seq = {D, U}},
    {.mapping = {L, F}, .result_seq = {Z, A}},
    {.mapping = {SCLN, F}, .result_seq = {G, E}},
    {.mapping = {Z, J}, .result_seq = {B, I}},
    {.mapping = {X, J}, .result_seq = {B, U}},
    {.mapping = {C, J}, .result_seq = {G, I}},
    {.mapping = {M, F}, .result_seq = {B, A}},
    {.mapping = {COMM, F}, .result_seq = {D, E}},
    {.mapping = {SLSH, F}, .result_seq = {D, I}},
    {.mapping = {R, M}, .result_seq = {P, E}},
    {.mapping = {G, M}, .result_seq = {P, O}},
    {.mapping = {Z, M}, .result_seq = {P, I}},
    {.mapping = {X, M}, .result_seq = {P, U}},
    {.mapping = {V, M}, .result_seq = {P, A}},

    // special
    {.mapping = {ENTER, C}, .result_seq = {DOT}},
    {.mapping = {ENTER, V}, .result_seq = {COMMA}},
    {.mapping = {SPACE, C}, .result_seq = {DOT}},
    {.mapping = {SPACE, V}, .result_seq = {COMMA}},

    // 小書き
    {.mapping = { Y, B }, .result_seq = { X, O }},
    {.mapping = { T, N }, .result_seq = { X, A }},
    {.mapping = { Y, E }, .result_seq = { X, I }},
    {.mapping = { Y, A }, .result_seq = { X, E }},
    {.mapping = { T, SCLN }, .result_seq = { X, U }},
    {.mapping = { T, U }, .result_seq = { X, Y, U }},
    {.mapping = { T, J }, .result_seq = { X, Y, O }},
    {.mapping = { T, COMM }, .result_seq = { X, Y, A }},

    // 拗音拡張
    {.mapping = {A, I}, .result_seq = {S, Y, U}},
    {.mapping = {A, K}, .result_seq = {S, Y, O}},
    {.mapping = {A, COMM}, .result_seq = {S, Y, A}},

    {.mapping = {C, I}, .result_seq = {K, Y, U}},
    {.mapping = {C, K}, .result_seq = {K, Y, O}},
    {.mapping = {C, COMM}, .result_seq = {K, Y, A}},

    {.mapping = {SLSH, E}, .result_seq = {T, Y, U}},
    {.mapping = {SLSH, D}, .result_seq = {T, Y, O}},
    {.mapping = {SLSH, C}, .result_seq = {T, Y, A}},

    {.mapping = {G, I}, .result_seq = {N, Y, U}},
    {.mapping = {G, K}, .result_seq = {N, Y, O}},
    {.mapping = {G, COMM}, .result_seq = {N, Y, A}},

    {.mapping = {D, I}, .result_seq = {M, Y, U}},
    {.mapping = {D, K}, .result_seq = {M, Y, O}},
    {.mapping = {D, COMM}, .result_seq = {M, Y, A}},

    {.mapping = {Z, I}, .result_seq = {H, Y, U}},
    {.mapping = {Z, K}, .result_seq = {H, Y, O}},
    {.mapping = {Z, COMM}, .result_seq = {H, Y, A}},

    {.mapping = {X, I}, .result_seq = {R, Y, U}},
    {.mapping = {X, K}, .result_seq = {R, Y, O}},
    {.mapping = {X, COMM}, .result_seq = {R, Y, A}},
    
    {.mapping = {A, I, O}, .result_seq = {Z, Y, U}},
    {.mapping = {A, K, L}, .result_seq = {Z, Y, O}},
    {.mapping = {A, COMM, DOT}, .result_seq = {Z, Y, A}},

    {.mapping = {C, I, O}, .result_seq = {G, Y, U}},
    {.mapping = {C, K, L}, .result_seq = {G, Y, O}},
    {.mapping = {C, COMM, DOT}, .result_seq = {G, Y, A}},

    {.mapping = {SLSH, E, W}, .result_seq = {D, Y, U}},
    {.mapping = {SLSH, D, S}, .result_seq = {D, Y, O}},
    {.mapping = {SLSH, C, X}, .result_seq = {D, Y, A}},

    {.mapping = {Z, I, O}, .result_seq = {B, Y, U}},
    {.mapping = {Z, K, L}, .result_seq = {B, Y, O}},
    {.mapping = {Z, COMM, DOT}, .result_seq = {B, Y, A}},

    {.mapping = {Z, I, U}, .result_seq = {P, Y, U}},
    {.mapping = {Z, K, J}, .result_seq = {P, Y, O}},
    {.mapping = {Z, COMM, M}, .result_seq = {P, Y, A}},
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

// seqをkeycodeの順にsortする
void japanese_input_sort_sequence(uint32_t *seq, size_t size) {
    for (size_t i = 0; i < size - 1; ++i) {
        for (size_t j = i + 1; j < size; ++j) {
            if (seq[i] > seq[j]) {
                uint32_t temp = seq[i];
                seq[i] = seq[j];
                seq[j] = temp;
            }
        }
    }
}

static int on_japanese_input_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    LOG_DBG("position %d keycode 0x%02X", event.position, binding->param1);
    japanese_input_capture_keycode(binding->param1);

    // pressでは何も行わないことにする
    return ZMK_BEHAVIOR_OPAQUE;
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
    for (int i = 0; i < JAPANESE_INPUT_CAPTURE_SIZE; i++) {
      captured_keycodes[i] = 0;
    }

    // stadardize order of mapping
    int definition_size = sizeof(behavior_japanese_input_definitions) / sizeof(behavior_japanese_input_definitions[0]);
    for (int i = 0; i < definition_size; i++) {
      japanese_input_sort_sequence(behavior_japanese_input_definitions[i].mapping, JAPANESE_INPUT_CAPTURE_SIZE);
    }
  }
  initialized = true;
  return 0;
};

#define KP_INST(n)                                                                                 \
    BEHAVIOR_DT_INST_DEFINE(n, japanese_input_init, NULL, NULL, NULL, POST_KERNEL,                                \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_key_press_driver_api);

DT_INST_FOREACH_STATUS_OKAY(KP_INST)
