#include <Servo.h>

#include "Ultrasonic.h"

Ultrasonic ultrasonicForward(9, 8);
Ultrasonic ultrasonicRight(12, 13);

Servo rouedroite;
Servo rouegauche;

const int fowardThreshold = 7;
const int rightThresholdMin = 3;
const int rightThresholdMax = 5;
 
int arret = 1500;
 
// variable vitesse et de direction pour le servomoteur 
int temps = 0;
 
// Declaration de la variable pour la rampe de demarrage
int valeur_de_vitesse_demarrage;

unsigned long rampedemarrage = millis();
 
int distanceRightSensor;
int distanceForwardSensor;
 
 
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
  Serial.print(distanceRightSensor); // Distance will be 0 when out of set max range.
  Serial.println(" cm");

  delay(200);


}

void condition()
{
  if(distanceForwardSensor >= fowardThreshold && distanceRightSensor <= rightThresholdMax && distanceRightSensor >= rightThresholdMin)
  {
    forward();
  }

  else if (distanceForwardSensor < fowardThreshold && distanceRightSensor <= rightThresholdMax)
  {
    turnLeftSelf();
  }

  else if (distanceRightSensor >= rightThresholdMax)
  {
    turnRight();
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
    distanceRightSensor = ultrasonicRight.read(CM);

}


void forward() {
  rouedroite.writeMicroseconds(1300);   
  rouegauche.writeMicroseconds(1700);
}

void turnRight()
{
  rouedroite.writeMicroseconds(1500);   
  rouegauche.writeMicroseconds(1300);
}


void turnRightSelf()
{
  rouedroite.writeMicroseconds(1300);   
  rouegauche.writeMicroseconds(1300);

  //TODO
  delay(100);
}

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

