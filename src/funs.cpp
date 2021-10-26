// SPDX-FileCopyrightText: 2021 Sebastian Serewa <neos32.project@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#include "include/main.h"

void funs_process(bool i_bIncrease, int& var, int step_size) {
    if (true == i_bIncrease) {
        var += step_size;
        if (var > 0xFF)
            var = 0xFF;
    }
    else {
        var -= step_size;
        if (var < 0)
            var = 0;
    }
}


void funs_show_rgb_led_set_1ch(Adafruit_NeoPixel& pixels, uint32_t color) {
    for (int i = 0; i < WS2812B_LED_COUNT; i++)
        pixels.setPixelColor(i, color);

    pixels.show();
}

void funs_show_led_start(Adafruit_NeoPixel& pixels) {

    for (int i = 0; i < WS2812B_LED_COUNT; i++) { // For each pixel...
        pixels.setPixelColor(i, pixels.Color(0, 150, 0));

        pixels.show();

        delay(20);
    }
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.show();
}


void funs_show_rgb_led_beep(Adafruit_NeoPixel& pixels, int new_app_state, int value) {
    uint32_t color;

    if (APP_STATE_LED_OFF != new_app_state) {
        switch (new_app_state) {
        case APP_STATE_LED_RED_SETTING:
            color = pixels.Color(value, 0, 0);
            break;
        case APP_STATE_LED_GREEN_SETTING:
            color = pixels.Color(0, value, 0);
            break;
        case APP_STATE_LED_BLUE_SETTING:
            color = pixels.Color(0, 0, value); // Send the updated pixel colors to the hardware.
            break;
        case APP_STATE_LED_WHITE_LUM_SETTING:
            color = pixels.Color(value, value, value);
            break;
        }
        funs_show_rgb_led_set_1ch(pixels, color);

        delay(900);
    }

    pixels.clear();
    pixels.show();
}

const char* funs_get_app_state_name(int new_app_state) {
    switch (new_app_state) {
    case APP_STATE_LED_RED_SETTING:
        return("APP_STATE_LED_RED_SETTING");

    case APP_STATE_LED_GREEN_SETTING:
        return("APP_STATE_LED_GREEN_SETTING");

    case APP_STATE_LED_BLUE_SETTING:
        return("APP_STATE_LED_BLUE_SETTING");

    case APP_STATE_LED_WHITE_LUM_SETTING:
        return("APP_STATE_LED_WHITE_LUM_SETTING");

    case APP_STATE_LED_OFF:
        return("APP_STATE_LED_OFF");

    default:
        return("???");
    };
};
