
# 8 Channel PWM Transmitter with Trim Buttons

This project builds upon the 8-channel PWM transmitter with additional trim button functionality, allowing for fine-tuning of roll, pitch, and yaw values. It uses an Arduino and nRF24L01 module for wireless transmission, with trim values saved in EEPROM to retain adjustments across restarts.

## Features

- 8 PWM Channels: Throttle, Yaw, Pitch, Roll, and 4 auxiliary channels
- Roll, Pitch, and Yaw trim adjustments with EEPROM storage
- LED feedback for connection status
- Uses nRF24L01 for wireless communication
- Joystick controls for throttle, yaw, pitch, and roll
- 4 auxiliary switches for extra controls

## Hardware Requirements

- **Arduino** (Uno, Nano, or Mega)
- **nRF24L01** module
- Joystick module (analog input for throttle, yaw, pitch, roll)
- 6 push buttons for trim adjustment (Roll, Pitch, and Yaw increase/decrease)
- 4 auxiliary switches for additional controls
- LED for connection status
- External 3.3V power supply for nRF24L01 (if required)

## Software Requirements

- **Arduino IDE**
- Libraries:
  - `SPI.h`
  - `nRF24L01.h`
  - `RF24.h`
  - `EEPROM.h`

## Circuit Diagram

*(Insert a circuit diagram here or link to an image URL if available)*

### Pin Connections

- **nRF24L01 Module**:
  - CE → Arduino Pin 9
  - CSN → Arduino Pin 10
  - SCK → Arduino Pin 13
  - MOSI → Arduino Pin 11
  - MISO → Arduino Pin 12
  - GND → GND
  - VCC → 3.3V

- **Joystick**:
  - X-axis → A0 (Yaw)
  - Y-axis → A1 (Throttle)
  - Z-axis → A3 (Roll)
  - Additional axis → A6 (Pitch)

- **Trim Buttons**:
  - Roll Increment → Pin 1
  - Roll Decrement → Pin 2
  - Pitch Increment → Pin 3
  - Pitch Decrement → Pin 4
  - Yaw Increment → Pin 5
  - Yaw Decrement → Pin 6

- **Auxiliary Switches**:
  - Aux1 → A4
  - Aux2 → A5
  - Aux3 → Pin 7
  - Aux4 → Pin 8

- **Status LED**: Connected to Pin A2

## Code Explanation

The code initializes the nRF24L01 module and sets up wireless transmission parameters. It reads joystick values and auxiliary switch states, and adjusts roll, pitch, and yaw based on trim button input. Trim adjustments are stored in EEPROM, preserving them even after a reset.

### Key Functions

- **ResetData()**: Resets signal values to defaults.
- **loadTrimValues()**: Loads saved trim values from EEPROM.
- **saveTrimValues()**: Stores updated trim values to EEPROM when adjusted.
- **mapJoystickValues()**: Maps joystick input values to PWM values, with an option to reverse the input.
- **handleTrimButtons()**: Reads button presses to adjust trim values for roll, pitch, and yaw.

### Trim Functionality

Trim values are adjusted using buttons:
- **Roll**: Buttons at Pins 1 (increase) and 2 (decrease)
- **Pitch**: Buttons at Pins 3 (increase) and 4 (decrease)
- **Yaw**: Buttons at Pins 5 (increase) and 6 (decrease)

Trim values are constrained between -127 and 127 and saved in EEPROM at addresses 0, 1, and 2.

## How to Use

1. **Setup Hardware**: Connect the Arduino, nRF24L01, and controls as per the circuit diagram.
2. **Upload the Code**: Load the provided code to your Arduino using the Arduino IDE.
3. **Operate**: Open the serial monitor to view transmitted PWM values. The status LED will indicate if the nRF24L01 module is connected.
4. **Adjust Trim**: Use the designated buttons to adjust trim for roll, pitch, and yaw. Trim values will be saved to EEPROM.

## Troubleshooting

- **No LED Feedback**: Check the nRF24L01 connections and ensure it’s receiving 3.3V power.
- **Trim Values Not Saving**: Verify that `EEPROM.h` is included and that EEPROM is properly functioning on your Arduino model.

## License

This project is open-source and available under the [MIT License](LICENSE).

## Author

Ayush Sharma  
[GitHub: Ayush-Zone](https://github.com/Ayush-Zone)
