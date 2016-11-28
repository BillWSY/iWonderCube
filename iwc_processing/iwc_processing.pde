import processing.serial.*;

static final int SIDE_SEL_PAGE = 0;
static final int MEMORY_SIDE = 1;
static final int SNAKE_SIDE = 2;
static final int MAZE_SIDE = 3;

// Serial protocol
static final byte MEMORY_SIDE_SEL = 0x30;
static final byte SNAKE_SIDE_SEL = 0x31;
static final byte MAZE_SIDE_SEL = 0x32;
static final byte NONE_SIDE_SEL = 0x3F;

// 0000xxxx, xxxx represents the index of the keys
static final byte MEMORY_SIDE_PREFIX = 0x00;

// 0x10: right, 0x11: up, 0x12: left, 0x13: down
static final byte SNAKE_SIDE_PREFIX = 0x10;

// 0x20: right, 0x21: up, 0x22: left, 0x23: down
static final byte MAZE_SIDE_PREFIX = 0x20;

// Directions, shared between snake and maze sides
static final byte IWC_RIGHT = 0x00;
static final byte IWC_UP = 0x01;
static final byte IWC_LEFT = 0x02;
static final byte IWC_DOWN = 0x03;

int currentSide = SIDE_SEL_PAGE;

SideSelectPage side_sel_page;
MemorySide memory_side;
SnakeSide snake_side;

Serial serialPort;

void setup() {
  String portName = Serial.list()[1];
  // Change the 0 to a 1 or 2 etc. to match your port
  serialPort = new Serial(this, portName, 9600);
  size(600, 600);
  frameRate(10);
  memory_side = new MemorySide(this);
  side_sel_page = new SideSelectPage(this);
}

void draw() {
  background(204);
  byte controlCmd = 0;
  boolean hasCmd = false;
  if (serialPort.available() > 0) {
    controlCmd = (byte)serialPort.readChar();
    hasCmd = true;
  }
  if (currentSide == SIDE_SEL_PAGE) {
    
    currentSide = side_sel_page.getNextSide();
    if (hasCmd && (controlCmd == MEMORY_SIDE_SEL)) {
      side_sel_page.goMemorySide();
    } else if (hasCmd && (controlCmd == SNAKE_SIDE_SEL)) {
      // side_sel_page.goSnakeSide();
    } else if (hasCmd && (controlCmd == MAZE_SIDE_SEL)) {
      // side_sel_page.goMazeSide();
    }
    side_sel_page.draw();
  } else if (currentSide == MEMORY_SIDE) {
    if (memory_side.isFinished()) {
      side_sel_page.setMemoryFinished();
      currentSide = SIDE_SEL_PAGE;
    }
    if (hasCmd && ((controlCmd & 0xF0) == MEMORY_SIDE_PREFIX)) {
      memory_side.keypadDown(controlCmd & 0x0F);
    }
    memory_side.draw();
  } else if (currentSide == SNAKE_SIDE) {
    if (snake_side.isFinished()) {
      side_sel_page.setSnakeFinished();
      currentSide = SIDE_SEL_PAGE;
    }
    if (hasCmd && ((controlCmd & 0xF0) == SNAKE_SIDE_PREFIX)) {
      snake_side.goDir(controlCmd & 0x0F);
    }
    snake_side.draw();
  } else if (currentSide == MAZE_SIDE) {
    // if (maze_side.isFinished()) {
      // side_sel_page.setMazeFinished();
      // currentSide = SIDE_SEL_PAGE;
    // }
    if (hasCmd && ((controlCmd & 0xF0) == MAZE_SIDE_PREFIX)) {
      // maze_side.goDir(controlCmd & 0x0F);
    }
    // maze_side.draw();
  }
}