#pragma once

#include <Arduino.h>

class ButtonMonitor
{
public:
  ButtonMonitor(
    int pin,
    unsigned long pressDuration = 2000,
    bool repeat = false,
    void (*onPress)() = nullptr,
    void (*onRelease)() = nullptr,
    uint8_t mode = INPUT_PULLDOWN
  );
  void tick(unsigned long ms);

private:
  int pin;
  unsigned long pressDuration;
  bool repeat;
  void (*onPress)();
  void (*onRelease)();
  unsigned long powerPressedSince;
  bool pressed = false;
};
