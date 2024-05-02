# Configuration Guide for GHLive Controller Replacement

## File Modifications
### `buttons.h`
This file defines the pin mappings for the various buttons on the Guitar Hero Live controller. You may need to modify these if your button layout differs.

### `main.cpp`
Contains the main logic for the controller. 
Adjust the `#define` statements as needed based on your hardware setup. By default, it should work if the [soldering guide](ASSEMBLY.md) is followed.

## Customizing Button Mappings
Modify the `BUTTON_MAP` array in `buttons.h` to change the GPIO pins associated with each button on the ESP32.

Example:
```c
const ButtonDef BUTTON_MAP[] = {
    {BUTTON_1, GPIO_NUM_14}, // W1
    {BUTTON_2, GPIO_NUM_9},  // W2
    // Add or modify entries as needed
};
```

## Battery Management
The firmware includes battery voltage reading and management. 
Modify the `BATTERY_MIN` and `BATTERY_MAX` definitions in `main.cpp` to fit the battery specifications you are using.
The default settings should be fine if you're using regular AA batteries.

## Bluetooth Settings
The device name and other Bluetooth settings can be configured in main.cpp. 
Adjust these to customize how the device appears to other Bluetooth hosts.