#pragma once

class ButtonMonitor
{
public:
  ButtonMonitor(
    int pin, 
    bool repeat = false,
    unsigned long pressDuration = 2000, 
    void (*onPress)() = nullptr, 
    void (*onRelease)() = nullptr,
    unsigned char mode = 9
  );
  void tick(unsigned long ms);
};