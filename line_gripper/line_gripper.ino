#include <Servo.h>
const int leftB = 11; //motor left - going backwards A1
const int leftF = 10; // motor left - going forward A2
const int rightB = 6; //motor right - going backwards B2
const int rightF = 9; //motor right - going forward B1
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

void left(){
  analogWrite(leftF, 255);
  analogWrite(rightF, 180);
}

void right(){
  analogWrite(leftF, 180);
  analogWrite(rightF, 255);
}
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
void followLine(){
  readSensors();
  if( catchobject == 0){
      grippercatch();
      catchobject = 1;
    }
  if(IR2 > 600 || IR3 > 600 || IR4 > 600 || IR5 > 600 || IR6 > 600 || IR7 > 600){
      int leftValue = (IR5 + IR6 + IR4) / 3;
      int rightValue = (IR1 + IR8 + IR7) / 3;

    
      //if the sensor readings on the left are bigger, go slightly left - center the robot on the line
      if(leftValue > rightValue){ //checking if robot is reading more with sensors 5 and 6
        //if robot is more to the left, the right wheel is having more speed
        analogWrite(leftF, 255);
        analogWrite(rightF, 80);
      }else{
        //if robot is more to the right, the left wheel will have more speed
        analogWrite(leftF, 80);
        analogWrite(rightF, 255);
      }
    }
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

void readSensors(){
  IR1 = analogRead(A0);
  IR2 = analogRead(A1);
  IR3 = analogRead(A2);
  IR4 = analogRead(A4);
  IR5 = analogRead(A3);
  IR6 = analogRead(A5);
  IR7 = analogRead(A6);
  IR8 = analogRead(A7);
}
void setup(){
  Serial.begin(9600); 
  setupIRSensors();
  setupMotors();
}
void loop(){
  followLine();
}
