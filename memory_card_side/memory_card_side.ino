#include <Key.h>
#include <Keypad.h>

// Minimal interval between two consecutive checks
unsigned long CHECK_INTERVAL = 50;

// Const number of rows of keypad
const byte KEYPAD_ROWS_CNT = 4;
// Const number of columnss of keypad
const byte KEYPAD_COLUMNS_CNT = 4;

// Const array of pins used as output for rows of keypad
const byte KEYPAD_OUT_PINS[ROWS_CNT] = {2, 3, 4, 5};
// Const array of pins used as input for columnss of keypad
const byte KEYPAD_IN_PINS[COLUMNS_CNT] = {6, 7, 8, 9};

const int MEMORY_SIDE_SEL_BTN = A0;

// Serial protocol
const int MEMORY_SIDE_SEL = 0x30;
const int SNAKE_SIDE_SEL = 0x31;
const int MAZE_SIDE_SEL = 0x32;

// 0000xxxx, xxxx represents the index of the keys
const int MEMORY_SIDE_PREFIX = 0x00;

// 0x10: right, 0x11: up, 0x12: left, 0x13: down
const int SNAKE_SIZE_PREFIX = 0x10;

// 0x20: right, 0x21: up, 0x22: left, 0x23: down
const int MAZE_SIZE_PREFIX = 0x20;

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

// Runs once when pressing reset
void setup() {
  // Prepare serial port
  Serial.begin(9600);

  // Makes pin mode of memory side selection button
  pinMode(MEMORY_SIDE_SEL_BTN, INPUT);
  // TODO: set mode for snake and maze sides

  // Prepares keypad
  // Makes pin mode of inputs as input with internal pullup
  for(byte i=0; i < KEYPAD_COLUMNS_CNT; ++i) {
    pinMode(KEYPAD_IN_PINS[i], INPUT_PULLUP);
  }
  // Makes pin mode of outputs
  for (byte i = 0; i < KEYPAD_ROWS_CNT; ++i) {
    pinMode(KEYPAD_OUT_PINS[i], OUTPUT);
  }

  // TODO: Add more sides initialization code here

  // TODO: Define the default side (none)
}

byte lastKeyPadValue = NO_KEYPAD_PRESSED;

// Repeated called by the Arduino library
void loop() {
  delay(CHECK_INTERVAL);

  // TODO: Read from I2C slave, if any result, return the slave result instead
  // of local checks

  // First, handle side selection button pressing events
  if (digitalRead(MEMORY_SIDE_SEL_BTN)) {
    Serial.print(MEMORY_SIDE_SEL);
    return;
  } /* else if ..., add more sides */
 
  // Memory side events
  byte keyPadValue = getKeypad();
  if (lastKeyPadValue != keyPadValue && keyPadValue != NO_KEYPAD_PRESSED) {
    // Send the key index to PC
    Serial.print(MEMORY_SIDE_PREFIX | (keyPadValue & 0x0F));
    lastKeyPadValue = keyPadValue;
    return;
  } else {
    lastKeyPadValue = keyPadValue;
  }

}
