#include <Servo.h>
Servo servoLeft;
Servo servoRight;

void setup() {
  servoLeft.attach(9);
  servoRight.attach(10);
}

void loop() {
  forward();
  delay(2000);
  reverse();
  delay(2000);
  turnRight();
  delay(2000);
  turnLeft();
  delay(2000);
  stopRobot();
  delay(2000);
}

//motion routines for forward, back, left right, stop

void forward() {
  servoLeft.write(0);
  servoRight.write(180);
}

void reverse() {
  servoLeft.write(180);
  servoRight.write(0);
}

void turnRight() {
  servoLeft.write(180);
  servoRight.write(180);
}

void turnLeft() {
  servoLeft.write(0);
  servoRight.write(0);
}

void stopRobot() {
  servoLeft.write(90);
  servoRight.write(90);
}


