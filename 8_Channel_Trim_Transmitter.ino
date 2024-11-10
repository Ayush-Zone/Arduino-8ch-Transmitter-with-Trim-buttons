// 8 Channel PPM Receiver
// Ayush Sharma
// https://github.com/Ayush-Zone

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <EEPROM.h>

const uint64_t pipeOut = 0xE9E8F0F0E1LL;
RF24 radio(9, 10);

int ledPin = A2;  // Pin for connection status LED

// Pin assignments for trim buttons
const int rollIncButton = 1;
const int rollDecButton = 2;
const int pitchIncButton = 3;
const int pitchDecButton = 4;
const int yawIncButton = 5;
const int yawDecButton = 6;

// Trim values stored in EEPROM (addresses 0, 1, 2)
int rollTrim = 0;
int pitchTrim = 0;
int yawTrim = 0;

struct Signal {
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte aux1;
  byte aux2;
  byte aux3;
  byte aux4;
};
Signal data;

// Reset data to default values
void ResetData() {
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.aux1 = 0;
  data.aux2 = 0;
  data.aux3 = 0;
  data.aux4 = 0;
}

// Load saved trim values from EEPROM
void loadTrimValues() {
  rollTrim = EEPROM.read(0) - 127;
  pitchTrim = EEPROM.read(1) - 127;
  yawTrim = EEPROM.read(2) - 127;
}

// Save updated trim values to EEPROM
void saveTrimValues() {
  EEPROM.write(0, rollTrim + 127);
  EEPROM.write(1, pitchTrim + 127);
  EEPROM.write(2, yawTrim + 127);
}

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Set trim button pins as input with pull-up resistors
  pinMode(rollIncButton, INPUT_PULLUP);
  pinMode(rollDecButton, INPUT_PULLUP);
  pinMode(pitchIncButton, INPUT_PULLUP);
  pinMode(pitchDecButton, INPUT_PULLUP);
  pinMode(yawIncButton, INPUT_PULLUP);
  pinMode(yawDecButton, INPUT_PULLUP);

  radio.begin();
  radio.stopListening();
  radio.openWritingPipe(pipeOut);
  radio.setAutoAck(false);
  radio.setCRCLength(RF24_CRC_8);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(108);
  ResetData();

  // Load trim values from EEPROM
  loadTrimValues();

  delay(1000);

  // Check if NRF24L01 module is connected
  if (radio.isChipConnected()) {
    digitalWrite(ledPin, HIGH);  // Turn on LED to indicate successful connection
  } else {
    digitalWrite(ledPin, LOW);  // Turn off LED if connection failed
  }
}

// Map joystick values with optional reversing
int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse) {
  val = constrain(val, lower, upper);
  if (val < middle)
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);
  return (reverse ? 255 - val : val);
}

// Check button presses and update trim values accordingly
void handleTrimButtons() {
  // Roll trim
  if (digitalRead(rollIncButton) == LOW) {
    rollTrim = min(rollTrim + 1, 127);
    saveTrimValues();
    delay(100);  // Simple debounce delay
  }
  if (digitalRead(rollDecButton) == LOW) {
    rollTrim = max(rollTrim - 1, -127);
    saveTrimValues();
    delay(100);
  }

  // Pitch trim
  if (digitalRead(pitchIncButton) == LOW) {
    pitchTrim = min(pitchTrim + 1, 127);
    saveTrimValues();
    delay(100);
  }
  if (digitalRead(pitchDecButton) == LOW) {
    pitchTrim = max(pitchTrim - 1, -127);
    saveTrimValues();
    delay(100);
  }

  // Yaw trim
  if (digitalRead(yawIncButton) == LOW) {
    yawTrim = min(yawTrim + 1, 127);
    saveTrimValues();
    delay(100);
  }
  if (digitalRead(yawDecButton) == LOW) {
    yawTrim = max(yawTrim - 1, -127);
    saveTrimValues();
    delay(100);
  }
}

void loop() {
  handleTrimButtons();  // Check for button presses to adjust trim

  // Read analog inputs and apply trim values
  data.throttle = mapJoystickValues(analogRead(A1), 75, 517, 872, false);

  data.yaw = constrain(mapJoystickValues(analogRead(A0), 0, 494, 1023, true) + yawTrim, 0, 255);
  data.roll = constrain(mapJoystickValues(analogRead(A3), 0, 517, 1023, false) + rollTrim, 0, 255);
  data.pitch = constrain(mapJoystickValues(analogRead(A6), 0, 510, 1023, false) + pitchTrim, 0, 255);

  // Read auxiliary switch states
  data.aux1 = !digitalRead(A4);
  data.aux2 = !digitalRead(A5);
  data.aux3 = !digitalRead(7);
  data.aux4 = !digitalRead(8);

  // Send the data over the radio
  radio.write(&data, sizeof(Signal));

  // Print data for debugging
  Serial.print("Throttle: "); Serial.print(data.throttle);
  Serial.print(" Yaw: "); Serial.print(data.yaw);
  Serial.print(" Roll: "); Serial.print(data.roll);
  Serial.print(" Pitch: "); Serial.print(data.pitch);
  Serial.print(" Aux1: "); Serial.print(data.aux1);
  Serial.print(" Aux2: "); Serial.print(data.aux2);
  Serial.print(" Aux3: "); Serial.print(data.aux3);
  Serial.print(" Aux4: "); Serial.println(data.aux4);

  delay(100);  // Small delay to avoid excessive updates
}
