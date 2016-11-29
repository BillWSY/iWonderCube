import cc.arduino.*;
import org.firmata.*;
import processing.serial.*;

class MazeSide {
  PApplet parent;
    
  public MazeSide(PApplet theParent) {
    parent = theParent;
  }
  
  int brushX = 300;  // Brush position X
  int brushY = 300;  // Brush position Y

  final int brushXStep = 10;  //  Brush step X
  final int brushYStep = 10;  //  Brush step Y
  final int brushWidth = 40;  //  Brush size X
  final int brushHeight = 40;  //  Brush size Y
  
  final int trapLeft = 500;
  final int trapUp = 400;
  final int trapWidth = 100;
  final int trapHeight = 100;
  
  boolean finished = false;

  void draw() {
    fill(255, 0, 0);
    ellipse(brushX, brushY, brushWidth, brushHeight);
    
    fill(0, 0, 255);
    rect(trapLeft + brushWidth, trapUp + brushHeight,
         trapWidth - brushWidth, trapHeight - brushHeight, 7);
    
    if (trapLeft <= brushX && brushX <= trapLeft + trapWidth &&
        trapUp <= brushY && brushY <= trapUp + trapHeight) {
      finished = true;
    }
  }

  boolean isFinished() {
    return finished;
  }
  
  void goDir(byte direction) {
    if (direction == IWC_UP) {
      brushY -= brushYStep;
    } else if (direction == IWC_DOWN) {
      brushY += brushYStep;
    } else if (direction == IWC_RIGHT) {
      brushX += brushXStep;
    } else if (direction == IWC_LEFT) {
      brushX -= brushXStep;
    }
    
    brushX = max(brushWidth / 2, brushX);
    brushY = max(brushHeight / 2, brushY);
    brushX = min(brushX, width - brushWidth / 2);
    brushY = min(brushY, height - brushHeight / 2);
  }
}