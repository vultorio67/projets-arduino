#include <Servo.h>

#include "Ultrasonic.h"

//prb de direction quand le robot detecte devant mais rien à gauche

//trig et echo
Ultrasonic ultrasonicForward(9, 8);
Ultrasonic ultrasonicRight(12, 13);

//the wheels
Servo rouedroite;
Servo rouegauche;

//captor
Servo servoDirection;

const int forwardThreshold = 8;
const int rightThresholdMin = 6;
const int rightThresholdMax = 9;
const int rightThresholdRotation = 20;
const int forwardgoBackAndTurnThreshOld = 85;

const int maxSameMoveDelay = 1500;
 
int arret = 1500;

//in case if needed
unsigned long startTime = millis();
 
int distanceLateralSensor;
int distanceForwardSensor;


int goBackAndTurnCount = 0;

////// à déterminer comment 
bool isRight = true;

bool isStart;

//0 = stop, 1 = forward, 2 = goBackAndTurn, 3 = turnRight, 4 = turnLeft
int movingMode = 0;
 
 
void setup() {

  Serial.begin(9600);
 
  rouedroite.attach(5);
  rouegauche.attach(6);

  rouedroite.writeMicroseconds(arret);   
  rouegauche.writeMicroseconds(arret);

  pinMode(2, INPUT_PULLUP);
 
}

void loop() {

  readDisance();

  if(digitalRead(2) == true)
  {
    isStart = !isStart;
    Serial.println("the robot is moving now !!!");
    delay(200);
  }

  if(isStart)
  {
    condition();
  }
  else {
    stop();
  }

  printInfo();

  delay(1);


}

void condition()
{
  //if nothing forward and correct distance on side
  if(distanceForwardSensor >= forwardThreshold && distanceLateralSensor <= rightThresholdMax && distanceLateralSensor >= rightThresholdMin)
  {
    forward();
    Serial.println("avance");
  }

  //if wall in front and wall on side
  else if (distanceForwardSensor < forwardThreshold && distanceLateralSensor <= rightThresholdMax)
  {
    if(isRight)
    {
      turnLeftSelf();
    }
    else {
      turnRightSelf();
    }
  }

  //if no wall detected
  else if (distanceForwardSensor > forwardThreshold && distanceLateralSensor > rightThresholdMax)
  {
    if(isRight)
    {
      turnRight();
    }
    else
    {
      turnLeft();
    }
  }

  //if wall in front and no wall on side
  else if (distanceForwardSensor < forwardThreshold && distanceLateralSensor > rightThresholdRotation)
  {
    if(isRight)
    {
      turnRightSelf();
    }
    else
    {
      turnLeftSelf();
    }
  }
  //if too close to the side wall
  else if(distanceLateralSensor < rightThresholdMin)
  {
    if(isRight)
    {
      turnLeft();
    }
    else
    {
      turnRight();
    }
  }

  //if lateral distance is too big
  else if (distanceLateralSensor >= rightThresholdMax)
  {
    if(isRight)
    {
      turnRight();
    }
    else{
      turnLeft();
    }
  }

    else if (distanceForwardSensor > forwardgoBackAndTurnThreshOld)
    {
      goBackAndTurn();
      delay(300);
      stop();
      delay(70);
    }
  
  else
  {
    stop();
  }

}

void readDisance()
{
    distanceForwardSensor = ultrasonicForward.read(CM);
    delay(50);
    distanceLateralSensor = ultrasonicRight.read(CM);

}



void forward() {
  if(movingMode != 1)
  {
    startTime = millis();
  }
  movingMode = 1;

  if(millis() - startTime > maxSameMoveDelay)
  {
        goBackAndTurn();
  }
  else {
    rouedroite.writeMicroseconds(1300);   
    rouegauche.writeMicroseconds(1700);
  }
}

//One wheel stoped
void turnRight()
{

  if(movingMode != 3)
  {
    startTime = millis();
  }
  movingMode = 3;

  if(millis() - startTime > maxSameMoveDelay)
  {
        goBackAndTurn();
  }
  else {
    rouedroite.writeMicroseconds(1475);   
    rouegauche.writeMicroseconds(1700);
  }

}

//One wheel stoped
void turnLeft()
{

    if(movingMode != 4)
  {
    startTime = millis();
  }
  movingMode = 4;

  if(millis() - startTime > maxSameMoveDelay)
  {
        goBackAndTurn();
  }
  else {
    rouedroite.writeMicroseconds(1300);   
    rouegauche.writeMicroseconds(1525);
  }


}

//turn on it self
void turnRightSelf()
{
  movingMode = -1;
  rouedroite.writeMicroseconds(1600);   
  rouegauche.writeMicroseconds(1600);
  delay(400);
  stop();
  delay(50);
}

//turn on it self
void turnLeftSelf()
{
  movingMode = -1;
  rouedroite.writeMicroseconds(1450);   
  rouegauche.writeMicroseconds(1450);
  delay(584);
  stop();
  delay(50);

}

void goBackAndTurn() {

  movingMode = 2;

  //pour tourner dans l'autre sens après un certaint count
  if(goBackAndTurnCount <= 2)
  {
    goBackAndTurnCount = goBackAndTurnCount+1;
    rouedroite.writeMicroseconds(1530);   
    rouegauche.writeMicroseconds(1400);
  }

  else 
  {
    goBackAndTurnCount = 0;
    rouedroite.writeMicroseconds(1600);   
    rouegauche.writeMicroseconds(1470);
  }

  delay(300);
}

void stop()
{
  movingMode = 0;
  rouedroite.writeMicroseconds(1500);   
  rouegauche.writeMicroseconds(1500);
}


void printInfo()
{
  Serial.print("Distance forward = ");
  Serial.print(distanceForwardSensor); // Distance will be 0 when out of set max range.
  Serial.println(" cm");
  Serial.print("Distance right = ");
  Serial.print(distanceLateralSensor); // Distance will be 0 when out of set max range.
  Serial.println(" cm");
}
