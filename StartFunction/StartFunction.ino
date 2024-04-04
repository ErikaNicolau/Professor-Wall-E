void startRace() {
  while (raceStart) {
    leds.fill(GREEN, 0, pixelCount);
    leds.show();
    getDistance();
    if (distance < 20 && !forwardCompleted) {
      Serial.println(distance);
      idle();
    } else {
      moveForwardDistance(40, 255);
      idle();
    }
    if (forwardCompleted && !turnLeftCompleted && !gripperClosed) {
      if (!turnLeftStarted) {
        Serial.println("Stop ");
        idle();
        Serial.println("Close");
        closeGripper();
        gripperClosed = true;
        delay(500);
        ticksLeft = 0;
        turnLeftStarted = true;
      }
      Serial.println("Turn Left ");
      startTurnLeft(34, 255);
      raceStart = false; // Stop the race loop after completing the initial actions
      gripperClosed = false;
    }
  }
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
