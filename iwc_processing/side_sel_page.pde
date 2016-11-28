import processing.sound.*;
import cc.arduino.*;
import org.firmata.*;

class SideSelectPage {
  PApplet parent;
  
  int nextSide = SIDE_SEL_PAGE;
  boolean finishedMemory = false;
  boolean finishedSnake = false;
  boolean finishedMaze = false;
  
  public SideSelectPage(PApplet theParent) {
    parent = theParent;
  }

  void setMemoryFinished() {
    finishedMemory = true;
    nextSide = SIDE_SEL_PAGE;
  }

  void setSnakeFinished() {
    finishedSnake = true;
    nextSide = SIDE_SEL_PAGE;
  }

  void setMazeFinished() {
    finishedMaze = true;
    nextSide = SIDE_SEL_PAGE;
  }

  void draw() {
    if (finishedMemory) {
      fill(255, 0, 0);
    } else {
      fill(0, 0, 255);
    }
    ellipse(100, 100, 120, 120);
    
    if (finishedSnake) {
      fill(255, 0, 0);
    } else {
      fill(0, 0, 255);
    }
    ellipse(250, 250, 120, 120);
    
    if (finishedMaze) {
      fill(255, 0, 0);
    } else {
      fill(0, 0, 255);
    }
    ellipse(400, 400, 120, 120);
  }
  
  void goMemorySide() {
    if (!finishedMemory) {
      nextSide = MEMORY_SIDE;
    }
  }
  
  void goSnakeSide() {
    if (!finishedSnake) {
      nextSide = SNAKE_SIDE;
    }
  }
  
  void goMazeSide() {
    if (!finishedMaze) {
      nextSide = MAZE_SIDE;
    }
  }
  
  int getNextSide() {
    return nextSide;
  }

  // Getter for finishing status
  boolean isFinished() {
    return finished;
  }
}