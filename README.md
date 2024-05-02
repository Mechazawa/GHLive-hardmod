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
 - You can use any ESP32 with enough pins
- [Guitar Hero Live controller](https://www.ebay.com/sch/i.html?_from=R40&_nkw=guitar+hero+live+contoller)
- [Basic soldering tools and skills](https://europe1.discourse-cdn.com/arduino/original/4X/6/5/f/65f9108b9d4b0c8300aa1e1ce5d76e2f1940d1ef.jpeg)
- [PlatformIO IDE](https://platformio.org)

### Installation
1. Clone the repository
2. Open the project in PlatformIO IDE.
3. Build and upload the firmware to your ESP32.

### Assembly

A step-by-step walthrough on how to assemble and solder this project can be found [here](ASSEMBLY.md)