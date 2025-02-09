#include <Servo.h>

#include "Ultrasonic.h"

Ultrasonic ultrasonicForward(9, 8);
Ultrasonic ultrasonicRight(12, 13);

//the wheels
Servo rouedroite;
Servo rouegauche;

//captor
Servo servoDirection;

const int fowardThreshold = 7;
const int rightThresholdMin = 3;
const int rightThresholdMax = 5;
 
int arret = 1500;

//in case if needed
unsigned long rampedemarrage = millis();
 
int distanceLateralSensor;
int distanceForwardSensor;

////// à déterminer comment 
bool isRight = true;
 
 
void setup() {

  Serial.begin(9600);
 
  rouedroite.attach(5);
  rouegauche.attach(6);
  rouedroite.writeMicroseconds(arret);   
  rouegauche.writeMicroseconds(arret);
 
}

void loop() {

  readDisance();

  condition();

  Serial.print("Distance forward = ");
  Serial.print(distanceForwardSensor); // Distance will be 0 when out of set max range.
  Serial.println(" cm");

  Serial.print("Distance right = ");
  Serial.print(distanceLateralSensor); // Distance will be 0 when out of set max range.
  Serial.println(" cm");

  if(isRight)
  {
    //à voir pour l'angle
    servoDirection.write(180);
  }
  else 
  {
    servoDirection.write(0);
  }

  delay(200);


}

void condition()
{
  //if nothing foward and correct distance on side
  if(distanceForwardSensor >= fowardThreshold && distanceLateralSensor <= rightThresholdMax && distanceLateralSensor >= rightThresholdMin)
  {
    forward();
  }

  //if wall in front and wall on side
  else if (distanceForwardSensor < fowardThreshold && distanceLateralSensor <= rightThresholdMax)
  {
    if(isRight)
    {
      turnLeftSelf();
    }
    else {
      turnRightSelf();
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
  rouedroite.writeMicroseconds(1300);   
  rouegauche.writeMicroseconds(1700);
}

//One wheel stoped
void turnRight()
{
  rouedroite.writeMicroseconds(1500);   
  rouegauche.writeMicroseconds(1300);
}

//One wheel stoped
void turnLeft()
{
  rouedroite.writeMicroseconds(1700);   
  rouegauche.writeMicroseconds(1500);
}

//turn on it self
void turnRightSelf()
{
  rouedroite.writeMicroseconds(1300);   
  rouegauche.writeMicroseconds(1300);

  //TODO
  delay(100);
}

//turn on it self
void turnLeftSelf()
{
  rouedroite.writeMicroseconds(1700);   
  rouegauche.writeMicroseconds(1700);

  //TODO
  delay(100);
}

void stop()
{
  rouedroite.writeMicroseconds(1500);   
  rouegauche.writeMicroseconds(1500);
}

