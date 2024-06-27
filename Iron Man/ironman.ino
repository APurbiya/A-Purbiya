#include <Servo.h>

Servo servoTop;
Servo servoBottom;

const int action_pin = 2;
int location = 31;

int bottom_closed = 107;
int top_closed = 167;
int bottom_open = 20;
int top_open = 20;

void setup() {
  pinMode(action_pin, INPUT_PULLUP);

  servoTop.attach(9);
  servoBottom.attach(10);

  // Set initial positions
  servoTop.write(top_closed);
  servoBottom.write(bottom_closed);
}

void loop() {
  int proximity = digitalRead(action_pin);

  if (proximity == LOW) {
    if (location > bottom_open) {
      moveServos(top_open, bottom_open);
      location = bottom_open;
      delay(600);
    } else {
      moveServos(top_closed, bottom_closed);
      location = bottom_closed;
      delay(600);
    }
  }
}

void moveServos(int topPos, int bottomPos) {
  servoTop.write(topPos);
  servoBottom.write(bottomPos);
}
