int getDistance() {
  // collect sensor values
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // calculate distance to object
  time = pulseIn(echoPin, HIGH);
  distance = time * 0.034 / 2; // take out time taken for ping to bounce back
  return distance;  // returns distance in cm
}

void moveForwardDistance(int ticksRemaining, int speed) {
  Serial.println("Moving forward");
  ticksLeft = 0; // Reset tick count
  while (ticksLeft <= ticksRemaining) {
    leds.fill(GREEN, 0, pixelCount);
    leds.show();
    // Set motors to move forward at specified speed
    analogWrite(motorLeftForward, speed);
    analogWrite(motorRightForward, speed);
    analogWrite(motorLeftBackward, 0);
    analogWrite(motorRightBackward, 0);
  }
  forwardCompleted = true; // Mark forward movement as completed
  idle(); // Stop the robot
  Serial.println("Forward movement completed");
}

void turnLeft(int ticksRemaining, int speed) {
  ticksLeft = 0;
  if (ticksLeft > ticksRemaining) {
    idle();
  } else {
    leds.clear();
    leds.setPixelColor(0, BLUE);
    leds.setPixelColor(3, BLUE);
    leds.show();
    Serial.println(ticksLeft);
    //setting right wheel on speed so robot will turn going forward, not backwards
    analogWrite(motorLeftForward, 0);
    analogWrite(motorRightForward, speed);
  }
}

void turnRight(int ticksRemaining, int speed) {
  ticksRight = 0;
  if (ticksRight > ticksRemaining) {
    idle();
  } else {
    leds.clear();
    leds.fill(BLUE, 1, 2);
    leds.show();
    Serial.println(ticksRight);
    //setting left wheel on speed so robot will turn going forward, not backwards
    analogWrite(motorLeftForward, speed);
    analogWrite(motorRightForward, 0);
  }
}

void tickLeft() {
  noInterrupts();
  ticksLeft++;
  interrupts();
}

void tickRight() {
  noInterrupts();
  ticksRight++;
  interrupts();
}

void idle() {
  // Stop the robot and turn LEDs to RED
  leds.fill(RED, 0, pixelCount);
  leds.show();
  analogWrite(motorLeftForward, 0);
  analogWrite(motorRightForward, 0);
  analogWrite(motorLeftBackward, 0);
  analogWrite(motorRightBackward, 0);
  Serial.println("Stop");
}


void avoidObject() {
  Serial.println("Avoiding object");
  leds.clear();
  leds.fill(ORANGE, 0, pixelCount);
  leds.show();
  boolean turningLeft = true;
  boolean turningRight = false;
  boolean changedPosition1 = false;
  boolean changedPosition2 = false;
  boolean changedPosition3 = false;
  if (turningLeft) {
    idle();
    Serial.println("Adjust Left ");
    turnLeft(20, 255);
    delay(500);
    turningLeft = false;
    changedPosition1 = true;
  }
  if (changedPosition1) {
    idle();
    Serial.println("Forward ");
    moveForwardDistance(30, 255);
    delay(500);
    changedPosition1 = false;
    turningRight = true;
  }
  if (turningRight) {
    idle();
    Serial.println("Adjust Right ");
    turnRight(20, 255);
    delay(500);
    turningRight = false;
    changedPosition2 = true;
  }
  if (changedPosition2) {
    idle();
    moveForwardDistance(20, 255);
    Serial.println("Forward again ");
    delay(500);
    idle();
    changedPosition1 = false;
    changedPosition2 = false;
    turningRight = false;
    turningLeft = false;
  }
  if (changedPosition3) {
    idle();
    turnLeft(20, 255);
    delay(500);
    idle();
    changedPosition3 = false;
    changedPosition1 = false;
    changedPosition2 = false;
    turningRight = false;
    turningLeft = false;
  }
}
