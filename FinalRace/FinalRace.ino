// Libraries
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

// Pin assignments
const int motorLeftBackward = 11;
const int motorLeftForward = 10;
const int motorRightBackward = 6;
const int motorRightForward = 9;
const int pixelPin = 12;
const int servoPin = 7;
const int echoPin = 4;
const int trigPin = 2;
const int rotaryLeft = 5;
const int rotaryRight = 3;

// Constants for sensor thresholds and positions
const int pixelCount = 4;
const int openPosition = 100;
const int closePosition = 40;
const int onBlack = 1000;
const int onWhite = 600;

// Global Variables for robot state
Adafruit_NeoPixel leds(pixelCount, pixelPin, NEO_RGB + NEO_KHZ800);
Servo servo;
int IR1, IR2, IR3, IR4, IR5, IR6, IR7, IR8;
volatile int ticksLeft = 0;
volatile int ticksRight = 0;
int time = 0;
long distance;
boolean forwardCompleted = false;
boolean turnLeftCompleted = false;
boolean turnLeftStarted = false;
boolean gripperClosed = false;
boolean raceStart = true;


// Color definitions
const uint32_t RED = leds.Color(255, 0, 0);
const uint32_t GREEN = leds.Color(0, 255, 0);
const uint32_t BLUE = leds.Color(0, 0, 255);
const uint32_t ORANGE = leds.Color(255, 165, 0);

// Forward declarations
int getDistance();
void setupMotorsAndSensors();
void setupIRSensors();
void readSensors();
void startRace();
void moveForwardDistance(int ticksRemaining, int speed);
void moveBackwardDistance(int ticksRemaining, int speed);
void startTurnLeft(int ticksRemaining, int speed);
void avoidObject();
void followLine();
void openGripper();
void closeGripper();
void idle();
bool isOnBlack(int sensor);
bool isOnWhite(int sensor);

void setup() {
  Serial.begin(9600);
  setupMotorsAndSensors();
  setupIRSensors();
  leds.begin();
  openGripper(); // Open the gripper at start
}

void loop() {
  leds.show();
  startRace();
  getDistance();
  while (getDistance() > 0) {
    if (distance < 15) {
      leds.clear();
      leds.fill(ORANGE, 0, pixelCount);
      leds.show();
      avoidObject();
    } else {
      followLine();
    }
  }
}
void setupMotorsAndSensors() {
  pinMode(motorLeftBackward, OUTPUT);
  pinMode(motorLeftForward, OUTPUT);
  pinMode(motorRightBackward, OUTPUT);
  pinMode(motorRightForward, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(rotaryRight), tickLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotaryLeft), tickRight, CHANGE);
  servo.attach(servoPin);
}

void setupIRSensors()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
}

void readSensors()
{
  IR1 = analogRead(A0);
  IR2 = analogRead(A1);
  IR3 = analogRead(A2);
  IR4 = analogRead(A4);
  IR5 = analogRead(A3);
  IR6 = analogRead(A5);
  IR7 = analogRead(A6);
  IR8 = analogRead(A7);
}

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

void startTurnLeft(int ticksRemaining, int speed) {
  Serial.println("Starting to turn left");
  ticksLeft = 0; // Reset tick count
  while (ticksLeft <= ticksRemaining) {
    leds.fill(BLUE, 0, pixelCount);
    leds.show();
    // Set motors to turn left
    analogWrite(motorLeftBackward, 0);
    analogWrite(motorRightForward, speed);
    analogWrite(motorLeftForward, 0);
    analogWrite(motorRightBackward, 0);
  }
  turnLeftCompleted = true; // Mark turn as completed
  idle(); // Stop the robot
  Serial.println("Left turn completed");
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

bool isOnBlack(int sensorValue) {
  return sensorValue <= onBlack && sensorValue > onWhite;
}

bool isOnWhite(int sensorValue) {
  return sensorValue <= onWhite;
}

void openGripper() {
  servo.write(openPosition); // Move servo to open position
  Serial.println("Gripper opened");
}

void closeGripper() {
  servo.write(closePosition); // Move servo to close position
  Serial.println("Gripper closed");
}

void avoidObject() {
  Serial.println("Avoiding object");
  boolean turningLeft = true;
  boolean turningRight = false;
  boolean changedPosition1 = false;
  boolean changedPosition2 = false;
  boolean changedPosition3 = false;
  if (turningLeft) {
    idle();
    Serial.println("Adjust Left ");
    turnLeft(40, 255);
    delay(500);
    turningLeft = false;
    changedPosition1 = true;
  }
  if (changedPosition1) {
    idle();
    Serial.println("Forward ");
    moveForwardDistance(34, 255);
    delay(500);
    changedPosition1 = false;
    turningRight = true;
  }
  if (turningRight) {
    idle();
    Serial.println("Adjust Right ");
    turnRight(26, 255);
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
    changedPosition2 = false;
    changedPosition3 = true;
  }
  if (changedPosition3) {
    idle();
    turnLeft(42, 255);
    delay(500);
    idle();
    changedPosition3 = false;
    changedPosition1 = false;
    changedPosition2 = false;
    turningRight = false;
    turningLeft = false;
  }
}
bool checkAllSensorsOnBlack() {
  return isOnBlack(IR1) && isOnBlack(IR2) && isOnBlack(IR3) && isOnBlack(IR4) &&
         isOnBlack(IR5) && isOnBlack(IR6) && isOnBlack(IR7) && isOnBlack(IR8);
}
bool checkDropSensorsOnBlack() {
  return isOnBlack(IR1) && isOnBlack(IR2) && isOnBlack(IR3) && isOnBlack(IR4);
}
void followLine() {
  readSensors();
  if (checkAllSensorsOnBlack()) {
    analogWrite(motorLeftForward, 255);
    analogWrite(motorRightForward, 255);
    delay(100);
    idle();
    readSensors();
    if (checkDropSensorsOnBlack()) {
      idle();
      openGripper();
      delay(1000);
      ticksLeft = 0;
      moveBackwardDistance(34, 255);
      idle();
      while (true) {}
    }
  } else if (isOnBlack(IR7) && isOnWhite(IR2) && isOnWhite(IR3) && isOnWhite(IR6)) {
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
  }
  else {
    leds.clear();
    leds.fill(GREEN, 0, 4);
    leds.show();
    analogWrite(motorLeftForward, 255);
    analogWrite(motorRightForward, 255);
  }
}
