#include <analogWrite.h>

// Motor
int rechtsVoor = 17;
int linksAchter = 18;
int linksVoor = 5;
int rechtsAchter = 16;

// Zwart wit sensor
int rightSensor = 39; 
int leftSensor = 34;
int rightSensorValue = 0;
int leftSensorValue = 0; 

//void driveForwardStraight(int rechtsVoor, int linksVoor, int rechtsAchter, int linksAchter) 
//{
//  analogWrite(rechtsVoor, 212);
//  analogWrite(linksVoor, 187);
//  analogWrite(rechtsAchter, LOW);
//  analogWrite(linksAchter, LOW); 
//}

void showTapeOutput(int sensor, int sensor2)
{
  rightSensorValue = digitalRead (sensor);
  leftSensorValue = digitalRead (sensor2);
  Serial.print ("Right sensor: ");
  Serial.print (rightSensorValue, DEC);
  Serial.println("");
  Serial.print ("Left sensor: ");
  Serial.print (rightSensorValue, DEC);
  Serial.println("");
}

void driveOverTape(int sensor, int sensor2) 
{
  rightSensorValue = digitalRead (sensor);
  leftSensorValue = digitalRead (sensor2);
  if (!(rightSensorValue) && !(leftSensorValue))
  {
    digitalWrite(linksVoor, LOW);
    digitalWrite(rechtsVoor, LOW);
    digitalWrite(linksAchter, LOW);
    digitalWrite(rechtsAchter, LOW);
  }
  else if (rightSensorValue && !(leftSensorValue))
  {
    digitalWrite(linksVoor, LOW);
    digitalWrite(rechtsVoor, HIGH);
    digitalWrite(linksAchter, LOW);
    digitalWrite(rechtsAchter, LOW);
  }
  else if (!(rightSensorValue) && leftSensorValue)
  {
    digitalWrite(linksVoor, HIGH);
    digitalWrite(rechtsVoor, LOW);
    digitalWrite(linksAchter, LOW);
    digitalWrite(rechtsAchter, LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
//  TCCR0B = TCCR0B & B11111000 | B00000010 ;
  pinMode(rechtsVoor, OUTPUT);
  pinMode(rechtsAchter, OUTPUT);
  pinMode(linksVoor, OUTPUT);
  pinMode(linksAchter, OUTPUT);
  pinMode(rightSensor, INPUT);
  pinMode(leftSensor, INPUT);
  Serial.begin (9600);
}

void loop() {
//  driveForwardStraight(rechtsVoor, linksVoor, rechtsAchter, linksAchter);
//  showTapeOutput(rightSensor, leftSensor);
  driveOverTape(rightSensor, leftSensor);
//  delay(1000);
}
