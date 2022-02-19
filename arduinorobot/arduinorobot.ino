#include <analogWrite.h>

// Motor
int rechtsVoor = 17;
int linksAchter = 18;
int linksVoor = 5;
int rechtsAchter = 16;

// Zwart wit sensor
int rightSensor = 39; 
int leftSensor = 34;
int sensorValue = 0;
int sensorValue2 = 0; 

void driveForwardStraight(int rechtsVoor, int linksVoor, int rechtsAchter, int linksAchter) 
{
  analogWrite(rechtsVoor, 212);
  analogWrite(linksVoor, 187);
  analogWrite(rechtsAchter, LOW);
  analogWrite(linksAchter, LOW); 
}

void showTapeOutput(int sensor, int sensor2)
{
  sensorValue = analogRead (Sensor);
  sensorValue2 = analogRead (Sensor2);
  Serial.print ("Right sensor: ");
  Serial.print (sensorValue, DEC);
  Serial.println("");
  Serial.print ("Left sensor: ");
  Serial.print (sensorValue2, DEC);
  Serial.println("");
}

void driveOverTape() 
{
  
}

void setup() {
  // put your setup code here, to run once:
  pinMode(rechtsVoor, OUTPUT);
  pinMode(rechtsAchter, OUTPUT);
  pinMode(linksVoor, OUTPUT);
  pinMode(linksAchter, OUTPUT);
  pinMode(Sensor, INPUT);
  Serial.begin (9600);
}

void loop() {
//  driveForwardStraight(rechtsVoor, linksVoor, rechtsAchter, linksAchter);
  showTapeOutput(rightSensor, leftSensor);
}
