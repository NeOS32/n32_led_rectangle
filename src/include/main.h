// SPDX-FileCopyrightText: 2021 Sebastian Serewa <neos32.project@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef Main_h_
#define Main_h_

#include <Arduino.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>

// pins 2,3 on mini are interrupt-supported
#define PIN_ENC_LEFT 2
#define PIN_ENC_RIGHT 3
#define SW 4
#define LED 13
#define PIN_RGB_LED 5

#define DELAY_MS_DEBOUNCE 50
#define DELAY_MS_RESET 5000
#define STEP_LUMINOSITY 20

#define WS2812B_LED_COUNT 3

#define DEBUG 1

enum app_state_e {
    APP_STATE_LED_WHITE_LUM_SETTING = 0,
    APP_STATE_LED_RED_SETTING = 1,
    APP_STATE_LED_GREEN_SETTING = 2,
    APP_STATE_LED_BLUE_SETTING = 3,
    APP_STATE_LED_OFF = 4,
    APP_STATE_MAX = 5
};

extern void funs_process(bool i_bIncrease, int& var, int step_size = 5);
extern void funs_show_led_start(Adafruit_NeoPixel& pixels);

extern void funs_show_rgb_led_beep(Adafruit_NeoPixel& pixels, int color, int value);
extern void funs_show_rgb_led_set_1ch(Adafruit_NeoPixel& pixels, uint32_t color);
extern const char* funs_get_app_state_name(int new_app_state);
extern void show_current_leds();
extern void funs_reset_triggered();

#endif // Main_h_
