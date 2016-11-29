// Daniel Shiffman
// http://codingrainbow.com
// http://patreon.com/codingrainbow
// Code for: https://youtu.be/AaGK-fj-BAM

class SnakeSide {
  PApplet parent;
  float x = 0;
  float y = 0;
  float xspeed = 1;
  float yspeed = 0;
  int total = 0;
  int scl = 20;
  PVector food;
  ArrayList<PVector> tail = new ArrayList<PVector>();

  SnakeSide(PApplet theParent) {
    parent = theParent;
    pickLocation();
  }

  boolean eat(PVector pos) {
    float d = dist(x, y, pos.x, pos.y);
    if (d < 1) {
      total++;
      return true;
    } else {
      return false;
    }
  }

  void dir(float x, float y) {
    xspeed = x;
    yspeed = y;
  }

  void death() {
    for (int i = 0; i < tail.size(); i++) {
      PVector pos = tail.get(i);
      float d = dist(x, y, pos.x, pos.y);
      if (d < 1) {
        println("starting over");
        total = 0;
        tail.clear();
      }
    }
  }

  int divCnt = 0;
  void update() {
    //println(total + " " + tail.size());
    divCnt = (divCnt + 1) % 4;
    if (divCnt != 0) {
      return;
    }
    
    if (total > 0) {
      if (total == tail.size() && !tail.isEmpty()) {
        tail.remove(0);
      }
      tail.add(new PVector(x, y));
    }

    x = x + xspeed*scl;
    y = y + yspeed*scl;

    x = constrain(x, 0, width-scl);
    y = constrain(y, 0, height-scl);
  }

  void show() {
    fill(255);
    for (PVector v : tail) {
      rect(v.x, v.y, scl, scl);
    }
    rect(x, y, scl, scl);
  }

  void pickLocation() {
    int cols = width/scl;
    int rows = height/scl;
    food = new PVector(floor(random(cols)), floor(random(rows)));
    food.mult(scl);
  }

  void draw() {

    background(51);

    if (eat(food)) {
      pickLocation();
    }
    death();
    update();
    show();

    fill(255, 0, 100);
    rect(food.x, food.y, scl, scl);
  }
  
  void goDir(byte direction) {
    if (direction == IWC_UP) {
      dir(0, -1);
    } else if (direction == IWC_DOWN) {
      dir(0, 1);
    } else if (direction == IWC_RIGHT) {
      dir(1, 0);
    } else if (direction == IWC_LEFT) {
      dir(-1, 0);
    }
  }
  
  boolean isFinished() {
    return total > 0;
  }

}


// Daniel Shiffman
// http://codingrainbow.com
// http://patreon.com/codingrainbow
// Code for: https://youtu.be/AaGK-fj-BAM

/*
Snake s;

void setup() {
  size(600, 600);
  frameRate(10);

  // Prepare Snake
  s = new Snake();
}

void mousePressed() {
  s.total++;
}

void draw() {
}

void keyPressed() {
  if (keyCode == UP) {
    s.dir(0, -1);
  } else if (keyCode == DOWN) {
    s.dir(0, 1);
  } else if (keyCode == RIGHT) {
    s.dir(1, 0);
  } else if (keyCode == LEFT) {
    s.dir(-1, 0);
  }
}*/