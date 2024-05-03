# GHLive Controller Hardmod

## Project Overview
This project replaces the main PCB of a Guitar Hero Live (GHLive) controller with an ESP32. 
It allows the controller to use Bluetooth to connect to games like Clone Hero, eliminating the need for a dongle.

## Features
- Bluetooth connectivity using ESP32
- Customizable button mappings
- Power management and battery level reporting

## Getting Started
### Requirements
- [ESP32 board](https://www.tinytronics.nl/en/development-boards/microcontroller-boards/with-wi-fi/wemos-s3-mini-esp32-s3-v1.0.0)
- [Guitar Hero Live controller](https://www.ebay.com/sch/i.html?_from=R40&_nkw=guitar+hero+live+contoller)
- [Basic soldering tools and skills](https://europe1.discourse-cdn.com/arduino/original/4X/6/5/f/65f9108b9d4b0c8300aa1e1ce5d76e2f1940d1ef.jpeg)
- [PlatformIO IDE](https://platformio.org)

### Installation
1. Clone the repository
2. Open the project in PlatformIO IDE.
3. Build and upload the firmware to your ESP32.

### Assembly
Following the defined pins in `buttons.h` should be enough to wire up the guitar.
Keep in mind that all signals are `INPUT_PULLDOWN`, meaning that some PCBs need to have their ground wired to 3.3 volts.
Asside from the power button and the whammy it does not matter what GPIO the buttons are wired to.
