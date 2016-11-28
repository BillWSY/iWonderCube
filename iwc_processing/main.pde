static final int SIDE_SEL_PAGE = 0;
static final int MEMORY_PAGE = 1;
static final int SNAKE_PAGE = 2;
static final int MAZE_PAGE = 3;

int currentSide = MEMORY_PAGE;
// int currentSide = SIDE_SEL_PAGE;

MemorySide memory_side;

void setup() {
  size(600, 600);
  frameRate(10);
  memory_side = new MemorySide();
}

void draw() {
  if (currentSide == SIDE_SEL_PAGE) {
    // side_sel_page.draw()
    // currentSide = side_sel_page.getNextSide();
  } else if (currentSide == MEMORY_PAGE) {
    memory_side.draw();
    if (memory_side.isFinished()) {
      // side_sel_page.setMemoryFinished();
      currentSide = side_sel_page;
    }
  }
}
