#include <Arduino.h>
#include <BleGamepad.h>

typedef struct
{
    uint8_t gamepad;
    gpio_num_t gpio;
} ButtonDef;

#define DISCONNECT_BUTTON GPIO_NUM_1
#define POWER_BUTTON_GPIO GPIO_NUM_4
#define BATTERY_GPIO GPIO_NUM_16
#define WHAMMY_GPIO GPIO_NUM_2

const gpio_num_t PLAYER_LEDS[] = {
    GPIO_NUM_35, 
    GPIO_NUM_36, 
    GPIO_NUM_44, 
    GPIO_NUM_43
};

const ButtonDef BUTTON_MAP[] = {
    {BUTTON_1, GPIO_NUM_14}, // W1
    {BUTTON_2, GPIO_NUM_9},  // W2
    {BUTTON_3, GPIO_NUM_8},  // W3
    {BUTTON_4, GPIO_NUM_10}, // B1
    {BUTTON_5, GPIO_NUM_11}, // B2
    {BUTTON_6, GPIO_NUM_13}, // B3

    {BUTTON_8, GPIO_NUM_1},
    {BUTTON_9, GPIO_NUM_5},
    {BUTTON_10, GPIO_NUM_6},
    {BUTTON_11, GPIO_NUM_7},
    {BUTTON_12, GPIO_NUM_4},
    {BUTTON_13, GPIO_NUM_12},

    {BUTTON_14, GPIO_NUM_21},
    {BUTTON_15, GPIO_NUM_17},
    {BUTTON_16, GPIO_NUM_15},
    {BUTTON_7, GPIO_NUM_18},
};
