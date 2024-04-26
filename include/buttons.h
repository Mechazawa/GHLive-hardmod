#include <Arduino.h>
#include <BleGamepad.h>

typedef struct
{
  uint8_t gamepad;
  uint8_t gpio;
} ButtonDef;

// GPIO 2 === Whammy ADC

const ButtonDef BUTTON_MAP[] = {
    {BUTTON_1, 14}, // W1
    {BUTTON_2, 9},  // W2
    {BUTTON_3, 8},  // W3
    {BUTTON_4, 10}, // B1
    {BUTTON_5, 11}, // B2
    {BUTTON_6, 13}, // B3

    // {BUTTON_17, 1}, // Wake (power)
    {BUTTON_8, 4},
    {BUTTON_9, 5},
    {BUTTON_10, 6},
    {BUTTON_11, 7},
    {BUTTON_12, 4},
    {BUTTON_13, 12},

    {BUTTON_14, 16},
    {BUTTON_15, 17},
    {BUTTON_16, 15},
    {BUTTON_7, 18},
};

const char PLAYER_LEDS[] = {
    35, 36, 44, 43
};