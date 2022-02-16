#include <analogWrite.h>

int rechtsVoor = 17;
int linksAchter = 18;
int linksVoor = 5;
int rechtsAchter = 16;

void driveForwardStraight(int rechtsVoor, int linksVoor, int rechtsAchter, int linksAchter) 
{
  analogWrite(rechtsVoor, 212);
  analogWrite(linksVoor, 187);
  analogWrite(rechtsAchter, LOW);
  analogWrite(linksAchter, LOW); 
}

//void setup() {
//  // put your setup code here, to run once:
//  pinMode(rechtsVoor, OUTPUT);
//  pinMode(rechtsAchter, OUTPUT);
//  pinMode(linksVoor, OUTPUT);
//  pinMode(linksAchter, OUTPUT);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
////  driveForwardStraight();
//
//}

int Sensor = 39; 
int sensorValue = 0; 

void setup () {
  pinMode(Sensor, INPUT);
  Serial.begin (9600);
}
void loop () {
  sensorValue = analogRead (Sensor);
  Serial.println (sensorValue, DEC);
}
