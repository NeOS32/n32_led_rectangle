// SPDX-FileCopyrightText: 2021 Sebastian Serewa <neos32.project@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#include "include/main.h"
#include "include/eeprom_wear.h"

Encoder myEnc(PIN_ENC_LEFT, PIN_ENC_RIGHT);

static Adafruit_NeoPixel pixels(WS2812B_LED_COUNT, PIN_RGB_LED, NEO_GRB + NEO_KHZ800);

static myConfigData_s myCfg;
static eeprom_wear_s myE2prom;
static int next_eeprom_offset = 0;

void setup() {
    pinMode(SW, INPUT_PULLUP);

#ifdef DEBUG
    Serial.begin(9600);
    Serial.println("-= LedRectangle =-");
    Serial.print("Reading cfg ... ");
#endif // ! DEBUG

    if (-1 != (next_eeprom_offset = myE2prom.readCfg(myCfg))) {
#ifdef DEBUG
        Serial.print("sucess, offset= ");
        Serial.println(next_eeprom_offset);
#endif // ! DEBUG
    }
    else {
#ifdef DEBUG
        Serial.println("failure ;o/");
#endif // ! DEBUG
        funs_reset_triggered();
    };

    pixels.begin();
    pixels.clear(); // Set all pixel colors to 'off'

    funs_show_led_start(pixels);

    show_current_leds();

#ifdef DEBUG
    Serial.print("New app state: ");
    Serial.println(funs_get_app_state_name(myCfg.mode));
#endif // ! DEBUG
};


void funs_reset_triggered() {
    myCfg.r = myCfg.g = myCfg.b = 0x0;
    myCfg.main_lum = 0x80;
    myCfg.mode = APP_STATE_LED_WHITE_LUM_SETTING;
    next_eeprom_offset = myE2prom.writeCfg(myCfg, 0);
#ifdef DEBUG
    Serial.println("eeprom reseted!");
#endif // ! DEBUG
}

void funs_button_pressed() {
    myCfg.mode++;
    myCfg.mode %= APP_STATE_MAX;

    next_eeprom_offset = myE2prom.writeCfg(myCfg, next_eeprom_offset);

    funs_show_rgb_led_beep(pixels, myCfg.mode, 0x9F);

#ifdef DEBUG
    Serial.print("New app state: ");
    Serial.println(funs_get_app_state_name(myCfg.mode));
#endif // ! DEBUG

    show_current_leds();
}

static void show_current_leds() {
    uint32_t color = 0;

    if (APP_STATE_LED_OFF != myCfg.mode)
        color = pixels.Color(myCfg.r, myCfg.g, myCfg.b);

    funs_show_rgb_led_set_1ch(pixels, color);
}

void encoder_rotated(bool i_bIncrease) {
    switch (myCfg.mode) {
    case APP_STATE_LED_WHITE_LUM_SETTING:
        funs_process(i_bIncrease, myCfg.main_lum, STEP_LUMINOSITY);
        break;
    case APP_STATE_LED_RED_SETTING:
        funs_process(i_bIncrease, myCfg.r, STEP_LUMINOSITY);
        break;
    case APP_STATE_LED_GREEN_SETTING:
        funs_process(i_bIncrease, myCfg.g, STEP_LUMINOSITY);
        break;
    case APP_STATE_LED_BLUE_SETTING:
        funs_process(i_bIncrease, myCfg.b, STEP_LUMINOSITY);
        break;
    case APP_STATE_LED_OFF:
        break;
    }

    next_eeprom_offset = myE2prom.writeCfg(myCfg, next_eeprom_offset);

#ifdef DEBUG
    Serial.print("New offset:");
    Serial.println(next_eeprom_offset);
#endif // ! DEBUG

    show_current_leds();
};



void loop() {
    long newPosition = myEnc.read();

    static long oldPosition = newPosition;
    static long lPushStart = -1;
    static bool isPushed = false;

    if (newPosition != oldPosition) {
        encoder_rotated(oldPosition < newPosition);
        oldPosition = newPosition;
    }

    if (digitalRead(SW) == LOW) {
        long current_ms = millis();

        if (-1 != lPushStart) {
            if (true == isPushed) {
                if (current_ms > lPushStart)
                    if (current_ms - lPushStart > DELAY_MS_RESET) {
                        funs_reset_triggered();

                        lPushStart = current_ms;
                        isPushed = false;
                        while (digitalRead(SW) == LOW)
                            ; // hacking, but we need to wait for user to release
                    }
            }
            else
                if (current_ms - lPushStart > DELAY_MS_DEBOUNCE)
                    isPushed = true;
        }
        else // if (-1 == lPushStart)
            lPushStart = current_ms;
    }
    else {
        if (true == isPushed) { // was pushed & is now released
            funs_button_pressed();
            isPushed = false;
        }
        lPushStart = -1;
    }

    // debouncing ..
    delay(5);
}
