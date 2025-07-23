#include QMK_KEYBOARD_H
#include "version.h"
#include "print.h"
#define MOON_LED_LEVEL LED_LEVEL
#define ML_SAFE_RANGE SAFE_RANGE
#ifndef ZSA_SAFE_RANGE
#define ZSA_SAFE_RANGE SAFE_RANGE
#endif


// Persistent config
typedef union {
    uint32_t raw;
    struct {
        bool     autoshift_enabled :1;
    };
} user_config_t;

user_config_t user_config;

// Called during EEPROM reset
void eeconfig_init_user(void) {
  user_config.raw = 0;
  user_config.autoshift_enabled = false;
  eeconfig_update_user(user_config.raw);
  autoshift_disable();
}


enum custom_keycodes {
    RGB_SLD = ML_SAFE_RANGE,
    HSV_27_255_255,
    TOGGLE_AUTOSHIFT,
};

enum tap_dance_codes {
    DANCE_0,
    DANCE_UC_MODE,
};

// Unicode support
#define UNICODE_SELECTED_MODES UNICODE_MODE_MACOS, UNICODE_MODE_WINCOMPOSE

enum unicode_names {
    U_NOT_SIGN,
    U_EM_DASH,
    U_GBP,
    U_EUR,
    U_TM,
    U_SECTION,
    U_MICRO,
    U_ALPHA,
    U_BETA,
};

const uint32_t PROGMEM unicode_map[] = {
    [U_NOT_SIGN]  = 0x00AC,  // ¬
    [U_EM_DASH]   = 0x2014,  // —
    [U_GBP]       = 0x00A3,  // £
    [U_EUR]       = 0x20AC,  // €
    [U_TM]        = 0x2122,  // ™
    [U_SECTION]   = 0x00A7,  // §
    [U_MICRO]     = 0x00B5,  // µ
    [U_ALPHA]     = 0x03B1,  // α
    [U_BETA]      = 0x03B2,  // β
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_moonlander(
    KC_EQUAL,       KC_1,           KC_2,           KC_3,           KC_4,           KC_5,           KC_6,                           KC_5,           KC_6,           KC_7,           KC_8,           KC_9,           KC_0,           KC_MINUS,
    KC_TAB,         KC_Q,           KC_D,           KC_R,           KC_W,           KC_B,           KC_EQUAL,                       KC_MEH,         KC_J,           KC_F,           KC_U,           KC_P,           KC_SCLN,        KC_BSLS,
    KC_BSPC,        KC_A,           KC_S,           KC_H,           KC_T,           KC_G,           CW_TOGG,                        CW_TOGG,        KC_Y,           KC_N,           KC_E,           KC_O,           KC_I,           KC_QUOTE,
    KC_LEFT_SHIFT,  KC_Z,           KC_X,           KC_M,           KC_C,           KC_V,                                                           KC_K,           KC_L,           KC_COMMA,       KC_DOT,         KC_SLASH,       KC_RIGHT_SHIFT,
    KC_LEFT_CTRL,   KC_LEFT_ALT,    LT(2,KC_GRAVE), KC_LEFT,        KC_RIGHT,       KC_ESCAPE,                                                      KC_ESCAPE,      KC_UP,          KC_DOWN,        KC_LBRC,        KC_RBRC,        TD(DANCE_0),
    KC_SPACE,       KC_BSPC,        KC_LEFT_GUI,                    KC_RIGHT_GUI,   KC_DELETE,      KC_ENTER
  ),
  [1] = LAYOUT_moonlander(
    _______, _______, _______, _______, _______, _______, _______,      _______, _______, _______, _______,     _______, _______,   _______,
    _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    _______,      _______, KC_Y,    KC_U,    KC_I,        KC_O,    KC_P,      _______,
    _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    _______,      _______, KC_H,    KC_J,    KC_K,        KC_L,    KC_SCLN,   _______,
    _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                           KC_N,    KC_M,    KC_COMMA,    KC_DOT,  KC_SLASH,  _______,
    _______, _______, _______, _______, _______, _______,                        _______, _______, _______,     _______, _______,   TO(0),
    _______, _______, _______,      _______, _______, _______
  ),
  [2] = LAYOUT_moonlander(
    KC_MEDIA_PREV_TRACK,    KC_F1,          KC_F2,              KC_F3,          KC_F4,          KC_F5,              TOGGLE_AUTOSHIFT,               MOON_LED_LEVEL, KC_F6,          KC_F7,          KC_F8,          KC_F9,          KC_F10,             KC_F11,
    KC_MEDIA_NEXT_TRACK,    UM(U_NOT_SIGN), UM(U_EM_DASH),      UM(U_GBP),      UM(U_EUR),      UM(U_TM),           TD(DANCE_UC_MODE),              QK_BOOT,        KC_NUM,         KC_KP_7,        KC_KP_8,        KC_KP_9,        KC_AUDIO_VOL_UP,    KC_F12,
    KC_MEDIA_PLAY_PAUSE,    UM(U_SECTION),  UM(U_MICRO),        KC_INSERT,      KC_HOME,        KC_PAGE_UP,         LALT(KC_END),                   _______,        KC_SCRL,        KC_KP_4,        KC_KP_5,        KC_KP_6,        KC_AUDIO_VOL_DOWN,  KC_KP_ENTER,
    _______,                UM(U_ALPHA),    UM(U_BETA),         KC_DELETE,      KC_END,         KC_PGDN,                                                            KC_PSCR,        KC_KP_1,        KC_KP_2,        KC_KP_3,        KC_AUDIO_MUTE,      _______,
    _______,                _______,        _______,            _______,        _______,        RGB_MODE_FORWARD,                                                   RGB_TOG,        _______,        KC_KP_DOT,      KC_KP_0,        KC_EQUAL,           TO(0),
    RGB_VAD,                RGB_VAI,        TOGGLE_LAYER_COLOR, RGB_SLD,        RGB_HUD,        RGB_HUI
  ),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(2,KC_GRAVE):
            return TAPPING_TERM + 100;
        default:
            return TAPPING_TERM;
    }
}

extern rgb_config_t rgb_matrix_config;

// NOTE: Need to patch moonlander.c to use this function, since the call at the end is wrong - matrix_init_user is
// erroneously called instead so use that
// void keyboard_post_init_user(void) {
void matrix_init_user(void) {
    user_config.raw = eeconfig_read_user();
    if (user_config.autoshift_enabled) {
        autoshift_enable();
    } else {
        autoshift_disable();
    }
}


const uint8_t PROGMEM ledmap[][RGB_MATRIX_LED_COUNT][3] = {
    [1] = { {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255} },
    [2] = { {0,255,255}, {0,255,255}, {0,255,255}, {79,255,255}, {79,255,255}, {79,255,255}, {41,255,255}, {41,255,255}, {41,255,255}, {79,255,255}, {79,255,255}, {41,255,255}, {41,255,255}, {41,255,255}, {79,255,255}, {79,255,255}, {41,255,255}, {0,245,245}, {0,245,245}, {79,255,255}, {79,255,255}, {41,255,255}, {0,245,245}, {0,245,245}, {79,255,255}, {79,255,255}, {41,255,255}, {0,245,245}, {0,245,245}, {150,255,255}, {150,255,255}, {131,255,255}, {0,255,255}, {0,255,255}, {0,255,255}, {0,255,255}, {79,255,255}, {79,255,255}, {79,255,255}, {79,255,255}, {0,255,255}, {79,255,255}, {79,255,255}, {79,255,255}, {79,255,255}, {79,255,255}, {79,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {79,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {79,255,255}, {79,255,255}, {196,255,255}, {196,255,255}, {196,255,255}, {79,255,255}, {79,255,255}, {150,255,255}, {150,255,255}, {150,255,255}, {0,255,255}, {0,255,255}, {79,255,255}, {0,255,255}, {0,255,255}, {0,255,255}, {0,255,255} },
};

void set_layer_color(int layer) {
    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        HSV hsv = {
        .h = pgm_read_byte(&ledmap[layer][i][0]),
        .s = pgm_read_byte(&ledmap[layer][i][1]),
        .v = pgm_read_byte(&ledmap[layer][i][2]),
        };
        if (!hsv.h && !hsv.s && !hsv.v) {
            rgb_matrix_set_color( i, 0, 0, 0 );
        } else {
            RGB rgb = hsv_to_rgb( hsv );
            float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
            rgb_matrix_set_color( i, f * rgb.r, f * rgb.g, f * rgb.b );
        }
    }
}

bool rgb_matrix_indicators_user(void) {
    if (rawhid_state.rgb_control) {
        return false;
    }
    if (keyboard_config.disable_layer_led) { return false; }
    switch (biton32(layer_state)) {
        case 1:
        set_layer_color(1);
        break;
        case 2:
        set_layer_color(2);
        break;
        case 3:
        set_layer_color(3);
        break;
    default:
        if (rgb_matrix_get_flags() == LED_FLAG_NONE)
        rgb_matrix_set_color_all(0, 0, 0);
        break;
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TOGGLE_AUTOSHIFT:
            if (record->event.pressed) {
                autoshift_toggle();
                user_config.autoshift_enabled = get_autoshift_state();
                eeconfig_update_user(user_config.raw);
            }
            return false;
        case RGB_SLD:
            if (rawhid_state.rgb_control) {
                return false;
            }
            if (record->event.pressed) {
                rgblight_mode(1);
            }
            return false;
        case HSV_27_255_255:
            if (rawhid_state.rgb_control) {
                return false;
            }
            if (record->event.pressed) {
                rgblight_mode(1);
                rgblight_sethsv(27,255,255);
            }
            return false;
    }
    return true;
}


typedef struct {
    bool is_press_action;
    uint8_t step;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP,
    DOUBLE_HOLD,
    DOUBLE_SINGLE_TAP,
    MORE_TAPS
};

static tap dance_state[2];

uint8_t dance_step(tap_dance_state_t *state);

uint8_t dance_step(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (state->interrupted) return DOUBLE_SINGLE_TAP;
        else if (state->pressed) return DOUBLE_HOLD;
        else return DOUBLE_TAP;
    }
    return MORE_TAPS;
}


void dance_0_finished(tap_dance_state_t *state, void *user_data);
void dance_0_reset(tap_dance_state_t *state, void *user_data);

void dance_0_finished(tap_dance_state_t *state, void *user_data) {
    dance_state[0].step = dance_step(state);
    switch (dance_state[0].step) {
        case SINGLE_TAP: layer_move(1); break;
        case SINGLE_HOLD: register_code16(KC_RIGHT_CTRL); break;
        case DOUBLE_TAP: layer_move(2); break;
        case DOUBLE_SINGLE_TAP: layer_move(1); break;
    }
}

void dance_0_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    switch (dance_state[0].step) {
        case SINGLE_HOLD: unregister_code16(KC_RIGHT_CTRL); break;
    }
    dance_state[0].step = 0;
}


void dance_uc_mode_finished(tap_dance_state_t *state, void *user_data);
void dance_uc_mode_reset(tap_dance_state_t *state, void *user_data);

void dance_uc_mode_finished(tap_dance_state_t *state, void *user_data) {
    dance_state[DANCE_UC_MODE].step = dance_step(state);
    switch (dance_state[DANCE_UC_MODE].step) {
        case SINGLE_TAP: set_unicode_input_mode(UNICODE_MODE_WINCOMPOSE); break;
        case DOUBLE_TAP: set_unicode_input_mode(UNICODE_MODE_MACOS); break;
    }
}

void dance_uc_mode_reset(tap_dance_state_t *state, void *user_data) {
    wait_ms(10);
    dance_state[DANCE_UC_MODE].step = 0;
}

tap_dance_action_t tap_dance_actions[] = {
    [DANCE_0] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_0_finished, dance_0_reset),
    [DANCE_UC_MODE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance_uc_mode_finished, dance_uc_mode_reset),
};
