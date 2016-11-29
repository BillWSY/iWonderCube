#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Key.h>
#include <Keypad.h>
#include <utility/imumaths.h>
#include <Wire.h>

const int SLAVE_ADDR = 0x08;

// Minimal interval between two consecutive checks
unsigned long CHECK_INTERVAL = 50;
// Interval for reporting BNO sensor directions
unsigned long BNO_REPORT_INTERVAL = 100;
// Interval for reporting photo sensor directions
unsigned long PHOTO_REPORT_INTERVAL = 100;

// Const number of rows of keypad
const byte KEYPAD_ROWS_CNT = 4;
// Const number of columnss of keypad
const byte KEYPAD_COLUMNS_CNT = 4;

// Const array of pins used as output for rows of keypad
const byte KEYPAD_OUT_PINS[KEYPAD_ROWS_CNT] = {2, 3, 4, 5};
// Const array of pins used as input for columnss of keypad
const byte KEYPAD_IN_PINS[KEYPAD_COLUMNS_CNT] = {6, 7, 8, 9};

const int MEMORY_SIDE_SEL_BTN = A0;
const int SNAKE_SIDE_SEL_BTN = A1;
const int MAZE_SIDE_SEL_BTN = A2;

// Serial protocol

const int NOP = 0xFF;

const int MEMORY_SIDE_SEL = 0x30;
const int SNAKE_SIDE_SEL = 0x31;
const int MAZE_SIDE_SEL = 0x32;
const int NONE_SIDE_SEL = 0x3F;

// 0000xxxx, xxxx represents the index of the keys
const int MEMORY_SIDE_PREFIX = 0x00;

// 0x10: right, 0x11: up, 0x12: left, 0x13: down
const int SNAKE_SIDE_PREFIX = 0x10;

// 0x20: right, 0x21: up, 0x22: left, 0x23: down
const int MAZE_SIDE_PREFIX = 0x20;

// Directions, shared between snake and maze sides
const int IWC_RIGHT = 0x00;
const int IWC_UP = 0x01;
const int IWC_LEFT = 0x02;
const int IWC_DOWN = 0x03;

int currentSide = 0;
const int MEMORY_SIDE = 0x00;
const int SNAKE_SIDE = 0x01;
const int MAZE_SIDE = 0x02;

const byte NO_KEYPAD_PRESSED = 0xFF;
// Gets the index of the key pressed in the keypad. If none of the keys is
// pressed, returns NO_KEYPAD_PRESSED(0xFF). If multiple keys are pressed, the
// return value is undefined.
byte getKeypad() {
  byte col = 0, row = 0;
  byte result = NO_KEYPAD_PRESSED;
  // TODO: Wait a while
  for (byte i = 0; i < KEYPAD_ROWS_CNT; ++i) {
    digitalWrite(KEYPAD_OUT_PINS[i], LOW);
    // TODO: Wait a while
    for (byte j = 0; j < KEYPAD_COLUMNS_CNT; ++j) {
      if (digitalRead(KEYPAD_IN_PINS[j]) == LOW) {
        result = i << 2 | j;
        break;
      }
    }
    digitalWrite(KEYPAD_OUT_PINS[i], HIGH);
    if (result != NO_KEYPAD_PRESSED) {
      break;
    }
  }
  return result;
}

Adafruit_BNO055 bno = Adafruit_BNO055(55);

// Runs once when pressing reset
void setup() {
  // Prepare serial port
  Serial.begin(9600);
  Wire.begin();

  // Makes pin mode of selection button
  pinMode(MEMORY_SIDE_SEL_BTN, INPUT);
  pinMode(SNAKE_SIDE_SEL_BTN, INPUT);
  pinMode(MAZE_SIDE_SEL_BTN, INPUT);

  // Prepares keypad
  // Makes pin mode of inputs as input with internal pullup
  for(byte i=0; i < KEYPAD_COLUMNS_CNT; ++i) {
    pinMode(KEYPAD_IN_PINS[i], INPUT_PULLUP);
  }
  // Makes pin mode of outputs
  for (byte i = 0; i < KEYPAD_ROWS_CNT; ++i) {
    pinMode(KEYPAD_OUT_PINS[i], OUTPUT);
  }

  // Prepares BNO055
  if(!bno.begin()) {
    // TODO: send error code, instead of text to PC (repeatly)
    Serial.print(
       "Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);

  // TODO: Add more sides initialization code here
}

byte lastSideSelected = NONE_SIDE_SEL;
byte lastKeyPadValue = NO_KEYPAD_PRESSED;
unsigned long lastBnoReportTime;
unsigned long lastPhotoReportTime;

// Repeated called by the Arduino library
void loop() {
  delay(CHECK_INTERVAL);

  // Then, we can determine which direction should the snake controller go
  if (millis() > lastPhotoReportTime + PHOTO_REPORT_INTERVAL) {
    // Read from I2C slave, if any result, return the result to the PC
    Wire.requestFrom(SLAVE_ADDR, 1);  // request 1 byte from slave device
    if (Wire.available()) {
      byte c = Wire.read();
      if (c != NOP) {
        Serial.write(c);
        lastPhotoReportTime = millis();
      }
    }
  }

  // First, handle side selection button pressing events
  if (digitalRead(MEMORY_SIDE_SEL_BTN)) {
    if (lastSideSelected != MEMORY_SIDE_SEL) {
      Serial.write(MEMORY_SIDE_SEL);
    }
    lastSideSelected = MEMORY_SIDE_SEL;
  } else if (digitalRead(SNAKE_SIDE_SEL_BTN)) {
    if (lastSideSelected != SNAKE_SIDE_SEL) {
      Serial.write(SNAKE_SIDE_SEL);
    }
    lastSideSelected = SNAKE_SIDE_SEL;
  } else if (digitalRead(MAZE_SIDE_SEL_BTN)) {
    if (lastSideSelected != MAZE_SIDE_SEL) {
      Serial.write(MAZE_SIDE_SEL);
    }
    lastSideSelected = MAZE_SIDE_SEL;
  } else {
    lastSideSelected = NONE_SIDE_SEL;
  }

  // Memory side events
  byte keyPadValue = getKeypad();
  if (lastKeyPadValue != keyPadValue && keyPadValue != NO_KEYPAD_PRESSED) {
    // Send the key index to PC
    Serial.write(MEMORY_SIDE_PREFIX | (keyPadValue & 0x0F));
  }
  lastKeyPadValue = keyPadValue;

  // Then, we can determine which direction should the snake controller go
  if (millis() > lastBnoReportTime + BNO_REPORT_INTERVAL) {
    sensors_event_t event; 
    bno.getEvent(&event);
    if (30.0 < event.orientation.y && event.orientation.y < 90.0) {
      Serial.write(SNAKE_SIDE_PREFIX | IWC_RIGHT);
      lastBnoReportTime = millis();
    } else if (-90.0 < event.orientation.z && event.orientation.z < -30.0) {
      Serial.write(SNAKE_SIDE_PREFIX | IWC_UP);
      lastBnoReportTime = millis();
    } else if (-90.0 < event.orientation.y && event.orientation.y < -30.0) {
      Serial.write(SNAKE_SIDE_PREFIX | IWC_LEFT);
      lastBnoReportTime = millis();
    } else if (30.0 < event.orientation.z && event.orientation.z < 90.0) {
      Serial.write(SNAKE_SIDE_PREFIX | IWC_DOWN);
      lastBnoReportTime = millis();
    }
  }
}
