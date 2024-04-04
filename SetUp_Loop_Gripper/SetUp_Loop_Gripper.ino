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


void openGripper() {
  servo.write(openPosition); // Move servo to open position
  Serial.println("Gripper opened");
}

void closeGripper() {
  servo.write(closePosition); // Move servo to close position
  Serial.println("Gripper closed");
}
