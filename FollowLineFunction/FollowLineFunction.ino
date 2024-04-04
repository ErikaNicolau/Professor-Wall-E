bool isOnBlack(int sensorValue) {
  return sensorValue <= onBlack && sensorValue > onWhite;
}

bool isOnWhite(int sensorValue) {
  return sensorValue <= onWhite;
}

bool checkAllSensorsOnBlack() {
  return isOnBlack(IR1) && isOnBlack(IR2) && isOnBlack(IR3) && isOnBlack(IR4) &&
         isOnBlack(IR5) && isOnBlack(IR6) && isOnBlack(IR7) && isOnBlack(IR8);
}
void moveBackwardDistance(int ticksRemaining, int speed) {
  Serial.println("Moving backward");
  ticksLeft = 0; // Reset tick count
  while (ticksLeft <= ticksRemaining) {
    leds.fill(RED, 0, pixelCount);
    leds.show();
    // Set motors to move backward at specified speed
    analogWrite(motorLeftBackward, speed);
    analogWrite(motorRightBackward, speed);
    analogWrite(motorLeftForward, 0);
    analogWrite(motorRightForward, 0);
  }
  idle(); // Stop the robot

  Serial.println("Backward movement completed");
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

void followLine() {
  readSensors();
    if (checkAllSensorsOnBlack()) {
    analogWrite(motorLeftForward, 255);
    analogWrite(motorRightForward, 255);
    delay(100);
    idle();
    readSensors();
    if (checkAllSensorsOnBlack()) {
      idle();
      openGripper();
      delay(1000);
      ticksLeft = 0;
      moveBackwardDistance(34, 255);
      idle();
      while(true){}
    }
    }else if (isOnBlack(IR7) && isOnWhite(IR2) && isOnWhite(IR3) && isOnWhite(IR6)) {
    leds.clear();
    leds.setPixelColor(0, BLUE);
    leds.setPixelColor(3, BLUE);
    leds.setPixelColor(1, GREEN);
    leds.setPixelColor(2, GREEN);
    leds.show();
    analogWrite(motorLeftForward, 240);
    analogWrite(motorRightForward, 255);//EXTREME LEFT
  } else if (isOnBlack(IR2) && isOnBlack(IR3) && isOnWhite(IR6) && isOnWhite(IR7)) {
    leds.clear();
    leds.fill(GREEN, 0, 4);
    leds.show();
    analogWrite(motorLeftForward, 255);
    analogWrite(motorRightForward, 255);//FORWARD
  } else if (isOnWhite(IR2) && isOnWhite(IR3) && isOnWhite(IR7) && isOnBlack(IR6)) {
    leds.clear();
    leds.setPixelColor(0, GREEN);
    leds.setPixelColor(3, GREEN);
    leds.setPixelColor(1, BLUE);
    leds.setPixelColor(2, BLUE);
    leds.show();
    analogWrite(motorLeftForward, 255);
    analogWrite(motorRightForward, 240);//EXTREME RIGHT
  } else if (isOnBlack(IR1) && isOnBlack(IR8)) {
    leds.clear();
    leds.setPixelColor(0, GREEN);
    leds.setPixelColor(3, BLUE);
    leds.setPixelColor(1, GREEN);
    leds.setPixelColor(2, GREEN);
    leds.show();
    analogWrite(motorLeftForward, 250);
    analogWrite(motorRightForward, 255);//SMALL LEFT
  } else if (isOnBlack(IR4) && isOnBlack(IR5)) {
    leds.clear();
    leds.setPixelColor(0, GREEN);
    leds.setPixelColor(3, GREEN);
    leds.setPixelColor(1, GREEN);
    leds.setPixelColor(2, BLUE);
    leds.show();
    analogWrite(motorLeftForward, 255);
    analogWrite(motorRightForward, 250);//SMALL RIGHT
  } else {    
    leds.clear();
    leds.fill(GREEN, 0, 4);
    leds.show();
    analogWrite(motorLeftForward, 255);
    analogWrite(motorRightForward, 255);
  }
}
