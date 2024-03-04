#include <Servo.h>
const int leftB = 11; //motor left - going backwards A1
const int leftF = 10; // motor left - going forward A2
const int rightB = 6; //motor right - going backwards B2
const int rightF = 9; //motor right - going forward B1
int rightSpeed;
int leftSpeed;
Servo servo;
int pos=10;
int rightSensor ;
int leftSensor ;
int catchobject =0;
unsigned long startTime = 0;

int IR1;
int IR2;
int IR3;
int IR4;
int IR5;
int IR6;
int IR7;
int IR8;
int leftValue = 0;
int rightValue = 0;
int adjustSpeed = 0;

void goForward() {
  analogWrite(leftF, 250);
  analogWrite(rightF, 255);
}

//void left(){
//  analogWrite(leftF, 255);
//  analogWrite(rightF, 100);
//}
//
//void right(){
//  analogWrite(leftF, 180);
//  analogWrite(rightF, 255);
//}
void grippercatch(){
    for(pos=120;pos>=40;pos--){
    servo.write(pos);
    delay(20);
    }
}
void gripperdrop(){
    for(pos=40;pos<=120;pos++){
    servo.write(pos);
    delay(20);
    }
}
void stop(){
  digitalWrite(leftB, LOW);
  digitalWrite(rightB, LOW);
  digitalWrite(leftF, LOW);
  digitalWrite(rightF, LOW);
}
void readSensors(){
  IR1 = analogRead(A5);
  IR2 = analogRead(A4);
  IR3 = analogRead(A3);
  IR4 = analogRead(A1);
  IR5 = analogRead(A2);
  IR6 = analogRead(A0);
  IR7 = analogRead(A7);
  IR8 = analogRead(A6);
}
  
//    if(IR1 < 800 && IR2 < 800 && IR3 < 800 && IR4 > 800 && IR5 > 800 && IR6 < 800 && IR7 < 800 && IR8 < 800)
//    {goForward();}

float Kp = 0.2; // Adjusted proportional gain, potentially lower than before
int maxCorrection = 200; // Maximum allowable correction to prevent abrupt changes

void followLine(){
  readSensors();
  if( catchobject == 0){
      grippercatch();
      catchobject = 1;
    }
  int leftValue = (IR4 * 2 + IR5 * 3 + IR6) / 6; 
  int rightValue = (IR1 + IR2 * 2 + IR3 * 3) / 6; 
  int error = leftValue - rightValue;
  if(IR4 > 600 && IR5 > 600 && IR1 < 600 && IR2 < 600 && IR3 < 600 && IR6 < 600) {
    // If only the center sensors detect the line, move forward at full speed
    goForward();
  } else {
  int correction = error * Kp;
  
  // Limit the correction to maxCorrection for smoother adjustments
  correction = constrain(correction, -maxCorrection, maxCorrection);
   
  int leftMotorSpeed = constrain(255 - correction, 0, 255);
  int rightMotorSpeed = constrain(255 + correction, 0, 255);

  // Adjust motor speeds more smoothly
  adjustMotorsSmoothly(leftMotorSpeed, rightMotorSpeed);
}
}
void adjustMotorsSmoothly(int targetLeftSpeed, int targetRightSpeed){
  static int currentLeftSpeed = 255;
  static int currentRightSpeed = 255;
  int speedAdjustmentStep = 5; // Adjust this to control how quickly speeds are adjusted
  
  // Gradually adjust current speed towards target speed
  if(currentLeftSpeed < targetLeftSpeed) currentLeftSpeed += speedAdjustmentStep;
  else if(currentLeftSpeed > targetLeftSpeed) currentLeftSpeed -= speedAdjustmentStep;
  
  if(currentRightSpeed < targetRightSpeed) currentRightSpeed += speedAdjustmentStep;
  else if(currentRightSpeed > targetRightSpeed) currentRightSpeed -= speedAdjustmentStep;
  
  // Apply the adjusted speeds
  analogWrite(leftF, currentLeftSpeed);
  analogWrite(rightF, currentRightSpeed);
}


bool squareDetected(){
  if(IR1 > 800 && IR2 > 800 && IR3 > 800 && IR4 > 800 && IR5 > 800 && IR6 > 800 && IR7 > 800 && IR8 > 800){
    return true;
      if(catchobject == 1){
      gripperdrop();
      catchobject=2;
    }
  }
}

void setupMotors(){
  pinMode(leftB, OUTPUT);
  pinMode(leftF, OUTPUT);
  pinMode(rightB, OUTPUT);
  pinMode(rightF, OUTPUT); 
  startTime=millis();
  servo.attach(7);
  servo.write(0);
}

void setupIRSensors(){
 pinMode(A0, INPUT);
 pinMode(A1, INPUT);
 pinMode(A2, INPUT);
 pinMode(A3, INPUT);
 pinMode(A4, INPUT);
 pinMode(A5, INPUT);
 pinMode(A6, INPUT);
 pinMode(A7, INPUT);
}


void setup(){
  Serial.begin(9600); 
  setupIRSensors();
  setupMotors();
}
void loop(){
  followLine();
}
