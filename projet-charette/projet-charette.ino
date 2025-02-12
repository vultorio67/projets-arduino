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

const int fowardThreshold = 8;
const int rightThresholdMin = 6;
const int rightThresholdMax = 9;
const int rightThresholdRotation = 15;
 
int arret = 1500;

//in case if needed
unsigned long rampedemarrage = millis();
 
int distanceLateralSensor;
int distanceForwardSensor;

////// à déterminer comment 
bool isRight = true;

bool isStart;
 
 
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

  //if no wall detected
  else if (distanceForwardSensor > fowardThreshold && distanceLateralSensor > rightThresholdMax)
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
  else if (distanceForwardSensor < fowardThreshold && distanceLateralSensor > rightThresholdRotation)
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
  rouedroite.writeMicroseconds(1475);   
  rouegauche.writeMicroseconds(1700);
}

//One wheel stoped
void turnLeft()
{
  rouedroite.writeMicroseconds(1300);   
  rouegauche.writeMicroseconds(1525);
}

//turn on it self
void turnRightSelf()
{
  rouedroite.writeMicroseconds(1600);   
  rouegauche.writeMicroseconds(1600);
  delay(400);
  stop();
  delay(125);
}

//turn on it self
void turnLeftSelf()
{
  rouedroite.writeMicroseconds(1400);   
  rouegauche.writeMicroseconds(1400);
  delay(400);
  stop();
  delay(125);

}

void stop()
{
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
