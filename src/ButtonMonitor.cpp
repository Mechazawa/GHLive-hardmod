#include <ButtonMonitor.hpp>

ButtonMonitor::ButtonMonitor(int pin, unsigned long pressDuration, bool repeat, void (*onPress)(), void (*onRelease)(), uint8_t mode)
    : pin(pin), pressDuration(pressDuration), repeat(repeat), onPress(onPress), onRelease(onRelease), powerPressedSince(0)
{
  pinMode(pin, mode);
}

void ButtonMonitor::tick(unsigned long ms)
{
  if (digitalRead(pin)) {
    if (powerPressedSince == 0) {
      powerPressedSince = ms;
    } else if (ms - powerPressedSince > pressDuration) {
      if (onPress && !pressed) {
        onPress();
      }

      if (repeat) {
        powerPressedSince = 0;
      } else {
        pressed = true;
      }
    }
  } else {
    if (powerPressedSince != 0 && onRelease) {
      onRelease();
    }
    powerPressedSince = 0;
  }
}
