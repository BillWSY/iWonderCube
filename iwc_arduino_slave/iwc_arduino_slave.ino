// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

const int SLAVE_ADDR = 0x08;

const int NOP = 0xFF;

// 0x20: right, 0x21: up, 0x22: left, 0x23: down
const int MAZE_SIDE_PREFIX = 0x20;

// Directions, shared between snake and maze sides
const int IWC_RIGHT = 0x00;
const int IWC_UP = 0x01;
const int IWC_LEFT = 0x02;
const int IWC_DOWN = 0x03;

const int TOP_LIGHT_PORT = A3; // Photoresistor opposite of the buttons
const int LEFT_LIGHT_PORT = A2; // Photoresistor opposite of the RGB LED
const int RIGHT_LIGHT_PORT = A1; // Photoresistor closest to the RGB LED
const int BOTTOM_LIGHT_PORT = A0; // Photoresistor closest to the buttons

const int LIGHT_THRESHOLD = 512;

volatile byte lastOp = NOP;

void setup() {
  Wire.begin(SLAVE_ADDR);  // join i2c bus with address #8
  Wire.onRequest(requestEvent);  // register event callback
  pinMode(TOP_LIGHT_PORT, INPUT);
  pinMode(LEFT_LIGHT_PORT, INPUT);
  pinMode(RIGHT_LIGHT_PORT, INPUT);
  pinMode(BOTTOM_LIGHT_PORT, INPUT);
}

const int DIR_SENSITIVITY = 200;

void loop() {
  int u = analogRead(TOP_LIGHT_PORT);
  int d = analogRead(BOTTOM_LIGHT_PORT);
  int l = analogRead(LEFT_LIGHT_PORT);
  int r = analogRead(RIGHT_LIGHT_PORT);
  int sum = u + d + l + r;
  if (u < ((sum - u) / 3 - DIR_SENSITIVITY)) {
    lastOp = MAZE_SIDE_PREFIX | IWC_UP;
  } else if (d < ((sum - d) / 3 - DIR_SENSITIVITY)) {
    lastOp = MAZE_SIDE_PREFIX | IWC_DOWN;
  } else if (l < ((sum - l) / 3 - DIR_SENSITIVITY)) {
    lastOp = MAZE_SIDE_PREFIX | IWC_LEFT;
  } else if (r < ((sum - r) / 3 - DIR_SENSITIVITY)) {
    lastOp = MAZE_SIDE_PREFIX | IWC_RIGHT;
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(lastOp);
  lastOp = NOP;
}
