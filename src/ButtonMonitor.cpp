#include <Arduino.h>

class ButtonMonitor
{
public:
  ButtonMonitor(int pin, unsigned long pressDuration = 2000, bool repeat = false, void (*onPress)() = nullptr, void (*onRelease)() = nullptr, uint8_t mode = INPUT_PULLDOWN)
      : pin(pin), pressDuration(pressDuration), repeat(repeat), onPress(onPress), onRelease(onRelease), powerPressedSince(0)
  {
    pinMode(pin, mode);
  }

  void tick(unsigned long ms)
  {
    if (digitalRead(pin))
    {
      if (powerPressedSince == 0)
      {
        powerPressedSince = ms;
      }
      else if (ms - powerPressedSince > pressDuration)
      {
        if (onPress && !pressed)
        {
          onPress();
        }

        if (repeat) {
          powerPressedSince = 0;
        } else {
          pressed = true;
        }
      }
    }
    else
    {
      if (powerPressedSince != 0 && onRelease)
      {
        onRelease();
      }
      powerPressedSince = 0;
    }
  }

private:
  int pin;
  bool repeat;
  bool pressed = false;
  unsigned long pressDuration;
  unsigned long powerPressedSince;
  void (*onPress)();
  void (*onRelease)();
};