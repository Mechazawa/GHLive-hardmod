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

bool tickButtons()
{
  bool changedState = false;

  for (auto button : BUTTON_MAP)
  {
    auto pressed = digitalRead(button.gpio);

    if (pressed && !bleGamepad.isPressed(button.gamepad))
    {
      bleGamepad.press(button.gamepad);

      changedState = true;
    }
    else if (!pressed && bleGamepad.isPressed(button.gamepad))
    {
      bleGamepad.release(button.gamepad);

      changedState = true;
    }
  }

  return changedState;
}

void setup()
{
  for (auto button : BUTTON_MAP)
  {
    pinMode(button.gpio, INPUT_PULLDOWN);
  }

  for (auto pin : PLAYER_LEDS)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  for (auto pin : PLAYER_LEDS)
  {
    digitalWrite(pin, LOW);
    delay(200);
    // digitalWrite(pin, HIGH);
  }

  pinMode(WHAMMY_GPIO, ANALOG);
  pinMode(BATTERY_GPIO, ANALOG);
  analogReadResolution(12);
  esp_sleep_enable_ext0_wakeup(POWER_BUTTON_GPIO, HIGH);

  auto *config = new BleGamepadConfiguration();

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

void tickBattery()
{
  unsigned long now = millis();
  if (lastBatteryCheck + 1000 < now)
  {
    lastBatteryCheck = now;
    // messed up wiring coming in handy I guess
    float voltage = analogReadMilliVolts(BATTERY_GPIO) * (3.3 / 4095.0) * 2;
    float percentage = (voltage - batteryMin) / (batteryMax - batteryMin) * 100;
    bleGamepad.setBatteryLevel(constrain(percentage, 0, 100));
  }
}

void flashLeds(uint16_t duration)
{
  const bool state = (millis() / duration) % 2;

  for (auto led : PLAYER_LEDS)
  {
    digitalWrite(led, state);
  }
}

unsigned long powerPressedSince = 0;
void watchPowerButton()
{
  if (digitalRead(POWER_BUTTON_GPIO))
  {
    if (powerPressedSince == 0)
    {
      powerPressedSince = millis();
    }
    else if (millis() - powerPressedSince > 2000)
    {
      for (auto led : PLAYER_LEDS)
      {
        digitalWrite(led, LOW);
      }
      for (auto led : PLAYER_LEDS)
      {
        delay(300);
        digitalWrite(led, HIGH);
      }

      powerPressedSince = 0;
      digitalRead(POWER_BUTTON_GPIO); // sanity
      esp_deep_sleep_start();
    }
  }
  else
  {
    powerPressedSince = 0;
  }
}

void loop()
{
  if (bleGamepad.isConnected())
  {
    digitalWrite(PLAYER_LEDS[0], LOW);
    digitalWrite(PLAYER_LEDS[1], HIGH);
    digitalWrite(PLAYER_LEDS[2], HIGH);
    digitalWrite(PLAYER_LEDS[3], HIGH);

    tickBattery();
    tickButtons();

    bleGamepad.setAxes(analogRead(WHAMMY_GPIO), 0, 0, 0, 0, 0, 0, 0);
    // bleGamepad.setAxes(analogReadMilliVolts(BATTERY_GPIO), analogRead(BATTERY_GPIO), analogRead(WHAMMY.gpio), analogReadMilliVolts(WHAMMY.gpio), 0, 0, 0, 0);

    bleGamepad.sendReport();
  }
  else
  {
    flashLeds(400);
  }
  
  watchPowerButton();
}