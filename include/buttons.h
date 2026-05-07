#include <Arduino.h>
#include <BleGamepad.h>

typedef struct
{
    uint8_t gamepad;
    gpio_num_t gpio;
} ButtonDef;

#define DISCONNECT_BUTTON GPIO_NUM_25
#define POWER_BUTTON_GPIO GPIO_NUM_12
// #define BATTERY_GPIO GPIO_NUM_16
#define WHAMMY_GPIO GPIO_NUM_39

const gpio_num_t PLAYER_LEDS[] = {
    GPIO_NUM_19, 
    GPIO_NUM_35, 
    GPIO_NUM_33, 
    GPIO_NUM_34, 
};

const ButtonDef BUTTON_MAP[] = {
    {BUTTON_1, GPIO_NUM_2}, // W1
    {BUTTON_2, GPIO_NUM_0},  // W2
    {BUTTON_3, GPIO_NUM_4},  // W3
    {BUTTON_4, GPIO_NUM_32}, // B1
    {BUTTON_5, GPIO_NUM_21}, // B2
    {BUTTON_6, GPIO_NUM_22}, // B3

    {BUTTON_8, DISCONNECT_BUTTON}, // VB
    {BUTTON_9, GPIO_NUM_18}, // Start
    {BUTTON_10, GPIO_NUM_26}, // Select
    {BUTTON_11, POWER_BUTTON_GPIO},
    {BUTTON_12, GPIO_NUM_5}, // Strum up
    {BUTTON_13, GPIO_NUM_23}, // Strum down

    // {BUTTON_14, GPIO_NUM_}, // Up
    // {BUTTON_15, GPIO_NUM_}, // Down
    // {BUTTON_16, GPIO_NUM_}, // Left
    // {BUTTON_7, GPIO_NUM_}, // Right
};
