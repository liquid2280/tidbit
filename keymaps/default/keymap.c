/* Copyright 2018 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "remote_kb.h"
#include "bitc_led.h"

#define ____ _______
#define _BASE     0
#define _FUNC      1


enum custom_keycodes {
  PROG = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Base layer (numpad)
  [_BASE] = LAYOUT(
    LT(_FUNC, KC_PSLS), KC_PAST, KC_PMNS, 
    KC_P7, KC_P8, KC_P9,   KC_PPLS,
    KC_P4, KC_P5, KC_P6,   _______, //not used with 2U +
    KC_P1, KC_P2, KC_P3,   KC_PENT,
    KC_P0, ____,  KC_DOT,  _______  //not used with 2U ENT
  ),

  // Function layer (numpad)
  [_FUNC] = LAYOUT(
             ____, RGB_TOG, ____,
    KC_NLCK, ____, RGB_MOD, ____,
    _______, ____, RGB_HUI, ____,
    _______, ____, RGB_SAI, ____,
    PROG,    ____, RGB_VAI, ____ 
  ),
};

void matrix_init_user(void) {
  matrix_init_remote_kb();
  set_bitc_LED(LED_OFF);
}

void matrix_scan_user(void) {
  matrix_scan_remote_kb();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  process_record_remote_kb(keycode, record);

  switch(keycode) {
    case PROG:
      if (record->event.pressed) {
        rgblight_disable_noeeprom();
        bootloader_jump(); //jump to bootloader
      }
    break;

    default:
    break;
  }
  return true;
}

void encoder_update_user(uint8_t index, bool clockwise) {
  if (clockwise) {
    tap_code(KC_VOLU);
  } else {
    tap_code(KC_VOLD);
  }  
}

void led_set_kb(uint8_t usb_led) {
  if (usb_led & (1<<USB_LED_NUM_LOCK))
    set_bitc_LED(LED_DIM);
  else
    set_bitc_LED(LED_OFF);
}