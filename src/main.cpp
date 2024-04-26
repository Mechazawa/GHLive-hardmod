/*
 * This example turns the ESP32 into a Bluetooth LE gamepad that presses buttons and moves axis
 *
 * At the moment we are using the default settings, but they can be canged using a BleGamepadConfig instance as parameter for the begin function.
 *
 * Possible buttons are:
 * BUTTON_1 through to BUTTON_16
 * (16 buttons by default. Library can be configured to use up to 128)
 *
 * Possible DPAD/HAT switch position values are:
 * DPAD_CENTERED, DPAD_UP, DPAD_UP_RIGHT, DPAD_RIGHT, DPAD_DOWN_RIGHT, DPAD_DOWN, DPAD_DOWN_LEFT, DPAD_LEFT, DPAD_UP_LEFT
 * (or HAT_CENTERED, HAT_UP etc)
 *
 * bleGamepad.setAxes sets all axes at once. There are a few:
 * (x axis, y axis, z axis, rx axis, ry axis, rz axis, slider 1, slider 2)
 *
 * Library can also be configured to support up to 5 simulation controls
 * (rudder, throttle, accelerator, brake, steering), but they are not enabled by default.
 *
 * Library can also be configured to support different function buttons
 * (start, select, menu, home, back, volume increase, volume decrease, volume mute)
 * start and select are enabled by default
 */

#include <Arduino.h>
#include <BleGamepad.h>
#include <buttons.h>

BleGamepad bleGamepad;

uint_fast16_t whammyBuffer[WHAMMY_BUFFER_SIZE] = {};
uint_fast32_t whammyBucket = 0;
auto whammyBufferIndex = 0;

uint16_t tickWhammy(uint_fast16_t input) {
  input = input <= 2048 ? input : 2048 - (input - 2048);

  whammyBucket -= whammyBuffer[whammyBufferIndex];
  whammyBuffer[whammyBufferIndex] = input;
  whammyBucket += input;

  whammyBufferIndex = (whammyBufferIndex + 1) % WHAMMY_BUFFER_SIZE;

  return (uint16_t)(whammyBucket / WHAMMY_BUFFER_SIZE);
}

void monitorButtons() {
  for (auto button: BUTTON_MAP) {
    auto pressed = digitalRead(button.gpio);

    if (pressed && !bleGamepad.isPressed(button.gamepad)) {
      bleGamepad.press(button.gamepad);
    } else if (!pressed && bleGamepad.isPressed(button.gamepad)) {
      bleGamepad.release(button.gamepad);
    }
  }
}

void setup()
{
    for(auto button: BUTTON_MAP) {
      pinMode(button.gpio, INPUT_PULLDOWN);
    }

    for (auto pin: PLAYER_LEDS) {
      pinMode(pin, OUTPUT);
    }

    // for(int i = 0; i < 3; i++) {
    //   for (auto pin: PLAYER_LEDS) {
    //     digitalWrite(pin, HIGH);
    //   }
    //   delay(300);
    //   for (auto pin: PLAYER_LEDS) {
    //     digitalWrite(pin, LOW);
    //   }
    //   delay(300);
    // }

    // Whammy shenanigans
    // pinMode(WHAMMY.gpio, ANALOG);
    // analogReadResolution(12);

    auto* config = new BleGamepadConfiguration();

    config->setAutoReport(false);
    config->setControllerType(CONTROLLER_TYPE_GAMEPAD);
    config->setButtonCount(20);
    config->setAxesMax(4096);
    config->setAxesMin(0);
    config->setWhichAxes(true, true, true, true, false, false, false, false);

    bleGamepad.deviceName = "GHLive Controller";
    bleGamepad.begin(config);
    // The default bleGamepad.begin() above enables 16 buttons, all axes, one hat, and no simulation controls or special buttons
}

unsigned long lastBatteryCheck = 0;
const auto batteryMin = 2.4;
const auto batteryMax = 3.2;

void loop()
{
    // unsigned long now = millis();
    // if (lastBatteryCheck + 1000 < now) {
    //   lastBatteryCheck = now;
    //   // messed up wiring coming in handy I guess
    //   float voltage = analogReadMilliVolts(WHAMMY.gpio) * (3.3 / 4095.0) * 2;
    //   float percentage = (voltage - batteryMin) / (batteryMax - batteryMin) * 100;
    //   bleGamepad.setBatteryLevel(constrain(percentage, 0, 100)); 
    // }

    if (bleGamepad.isConnected())
    {
        // digitalWrite(PLAYER_LEDS[0], HIGH);
        monitorButtons();
        // bleGamepad.setAxes(tickWhammy(analogRead(WHAMMY.gpio)), 0, 0, 0, 0, 0, 0, 0);
        // bleGamepad.setAxes(analogReadMilliVolts(WHAMMY.gpio), 0, tickWhammy(analogRead(WHAMMY.gpio)), 0, 0, 0, 0, 0);

        bleGamepad.sendReport();
    }
}