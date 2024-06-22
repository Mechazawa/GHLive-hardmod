#include <Arduino.h>
#include <Preferences.h>
#include <BleGamepad.h>
#include <buttons.h>
#include <NimBLEDevice.h>
#include <ButtonMonitor.hpp>

#define INPUT_TIMEOUT (5 * 60 * 1000)
#define BATTERY_REPORT_INTERVAL (5 * 1000)
#define BATTERY_MIN 2.4
#define BATTERY_MAX 3.2

unsigned long lastBatteryCheck = 0;
unsigned long powerPressedSince = 0;
unsigned long lastInput = 0;
unsigned char playerNumber = 1;

BleGamepad bleGamepad;
Preferences preferences;

String getSerial();
void watchPowerButton();
void shutdown();
void disconnect();
void flashLeds(uint16_t duration);
void tickInput();
void tickBattery();
bool tickButtons();

String getSerial() {

  String serial;

  if (preferences.isKey("serial")) {
    serial = preferences.getString("serial");
  } else {
    static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < 6; ++i) {
      serial += alphanum[random(strlen(alphanum) - 1)];
    }

    preferences.putString("serial", serial);
  }

  return "GHLiveController-" + serial;
}

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

void tickBattery()
{
  unsigned long now = millis();

  if (lastBatteryCheck == 0 || lastBatteryCheck + BATTERY_REPORT_INTERVAL < now)
  {
    lastBatteryCheck = now;

    const float voltage = analogReadMilliVolts(BATTERY_GPIO) * (3.3 / 4095.0) * 2;
    const float percentage = ((voltage - BATTERY_MIN) / (BATTERY_MAX - BATTERY_MIN)) * 100;

    bleGamepad.setBatteryLevel(constrain(percentage, 0, 100));
  }
}

void tickInput()
{
  if (tickButtons() || lastInput == 0)
  {
    lastInput = millis();
  }
  else
  {
    const auto now = millis();
    if (now - lastInput > INPUT_TIMEOUT)
    {
      lastInput = 0;
      shutdown();
    }
  }

  bleGamepad.setAxes(analogRead(WHAMMY_GPIO), 0, 0, 0, 0, 0, 0, 0);
}

void flashLeds(uint16_t duration)
{
  const bool state = (millis() / duration) % 2;

  for (auto led : PLAYER_LEDS)
  {
    digitalWrite(led, state);
  }
}

void setPlayerIndicator(int player) {
  auto index = 0;

  for (const auto pin : PLAYER_LEDS) {
    digitalWrite(pin, index != player);
    index++;
  }
}

void disconnect()
{
  for (char i = 0; i < 20; i++)
  {
    delay(100);
    for (auto led : PLAYER_LEDS)
    {
      digitalWrite(led, i % 2);
    }
  }

  NimBLEDevice::deleteAllBonds();

  auto clients = NimBLEDevice::getClientList();

  for (auto client : *clients)
  {
    NimBLEDevice::deleteClient(client);
  }

  NimBLEDevice::deleteAllBonds();

  esp_restart();
}

void shutdown()
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

  esp_deep_sleep_start();
}

void setup()
{
  preferences.begin("gamepad");

  for (auto button : BUTTON_MAP)
  {
    pinMode(button.gpio, INPUT_PULLDOWN);
  }

  for (auto pin : PLAYER_LEDS)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(200);
  }

  pinMode(WHAMMY_GPIO, ANALOG);
  pinMode(BATTERY_GPIO, ANALOG);

  esp_sleep_enable_ext0_wakeup(POWER_BUTTON_GPIO, HIGH);

  analogReadResolution(12);

  auto *config = new BleGamepadConfiguration();

  config->setAutoReport(false);
  config->setButtonCount(20);
  config->setAxesMax(4096);
  config->setAxesMin(0);
  config->setWhichAxes(true, true, false, false, false, false, false, false);
  config->setSoftwareRevision((char *)"1");
  config->setSerialNumber(const_cast<char*>(getSerial().c_str()));

  bleGamepad.deviceName = "GHLive Controller";

  bleGamepad.begin(config);

  playerNumber = preferences.getUChar("player");

  if (digitalRead(DISCONNECT_BUTTON))
  {
    disconnect();
  }
}

void nextPlayer() {
  playerNumber = (playerNumber + 1) / (sizeof(PLAYER_LEDS) / sizeof(gpio_num_t));

  preferences.putUChar("player", playerNumber);
}

ButtonMonitor monitoredButtons[] = {
  ButtonMonitor(POWER_BUTTON_GPIO, 2000, false, shutdown),
  ButtonMonitor(DISCONNECT_BUTTON, 1000, true, nextPlayer),
};

void loop()
{
  unsigned long ms = millis();

  for (auto button: monitoredButtons) {
    button.tick(ms);
  }

  tickBattery();
  tickInput();

  if (bleGamepad.isConnected())
  {
    setPlayerIndicator(playerNumber);
  }
  else
  {
    flashLeds(400);
  }

  bleGamepad.sendReport();
}