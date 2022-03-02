#include <Arduino.h>
#include <analogWrite.h>
#include <MPU9250.h>
#include <Adafruit_VL53L0X.h>

// Motor
int rechtsVoor = 17;
int linksAchter = 18;
int linksVoor = 5;
int rechtsAchter = 16;
int isStarted = 0;

// Zwart wit sensor
int rightSensor = 39; 
int leftSensor = 34;
int rightSensorValue = 0;
int leftSensorValue = 0; 

//adafruit vl53l0x
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void driveForwardStraight(int rechtsVoor, int linksVoor, int rechtsAchter, int linksAchter) 
{
  analogWrite(rechtsVoor, LOW);
  analogWrite(linksVoor, LOW);
  analogWrite(rechtsAchter, 187);
  analogWrite(linksAchter, 193); 
}

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

//void driveOverTape(int sensor, int sensor2, int rechtsVoor, int linksAchter, int linksVoor, int rechtsAchter) 
//{
//  rightSensorValue = digitalRead (sensor);
//  leftSensorValue = digitalRead (sensor2);
//  if (!(rightSensorValue) && !(leftSensorValue))
//  {
//    analogWrite(linksVoor, 180);
//    analogWrite(rechtsVoor, 180);
//    analogWrite(linksAchter, LOW);
//    analogWrite(rechtsAchter, LOW);
//  }
//  else if (rightSensorValue && !(leftSensorValue))
//  {
//    analogWrite(linksVoor, 180);
//    analogWrite(rechtsVoor, LOW);
//    analogWrite(linksAchter, LOW);
//    analogWrite(rechtsAchter, 180);
//  }
//  else if (!(rightSensorValue) && leftSensorValue)
//  {
//    analogWrite(linksVoor, LOW);
//    analogWrite(rechtsVoor, 180);
//    analogWrite(linksAchter, 180);
//    analogWrite(rechtsAchter, LOW);
//  }
//  else 
//  {
//    analogWrite(linksVoor, 180);
//    analogWrite(rechtsVoor, 180);
//    analogWrite(linksAchter, LOW);
//    analogWrite(rechtsAchter, LOW);
//  }
//}

void driveOverTape(int sensor, int sensor2, int rechtsVoor, int linksAchter, int linksVoor, int rechtsAchter) 
{
  rightSensorValue = digitalRead (sensor);
  leftSensorValue = digitalRead (sensor2);
  if (!(rightSensorValue) && !(leftSensorValue))
  {
    analogWrite(linksVoor, 158);
    analogWrite(rechtsVoor, 163);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, LOW);
  }
  else if (rightSensorValue && !(leftSensorValue))
  {
    analogWrite(linksVoor, LOW);
    analogWrite(rechtsVoor, 138);
    analogWrite(linksAchter, 143);
    analogWrite(rechtsAchter, LOW);
  }
  else if (!(rightSensorValue) && leftSensorValue)
  {
    analogWrite(linksVoor, 138);
    analogWrite(rechtsVoor, LOW);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, 143);
  }
  else 
  {
    analogWrite(linksVoor, LOW);
    analogWrite(rechtsVoor, LOW);
    analogWrite(linksAchter, 143);
    analogWrite(rechtsAchter, 138);
  }
}

uint16_t measureDistance()
{
  VL53L0X_RangingMeasurementData_t measureDistance;

  if (measureDistance.RangeStatus != 4) 
  {
    return measureDistance.RangeMilliMeter;
  } 
  else 
  {
    Serial.println(" out of range ");
  }
  delay(100);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(rechtsVoor, OUTPUT);
  pinMode(rechtsAchter, OUTPUT);
  pinMode(linksVoor, OUTPUT);
  pinMode(linksAchter, OUTPUT);
  analogWrite(linksVoor, 0);
  analogWrite(rechtsVoor, 0);
  analogWrite(linksAchter, 0);
  analogWrite(rechtsAchter, 0);
  pinMode(rightSensor, INPUT);
  pinMode(leftSensor, INPUT);
  
  Serial.begin (115200);

  //Start serial monitor
  while (!Serial) 
  {
    delay(1);
  }

  //check if VL53L0X starts up
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  delay(2000);
}

void loop() {
  if (!isStarted)
  {
    driveForwardStraight(rechtsVoor, linksVoor, rechtsAchter, linksAchter);
    delay(1000);
    isStarted = 1;
  }
//  showTapeOutput(rightSensor, leftSensor);
  driveOverTape(rightSensor, leftSensor, rechtsVoor, linksAchter, linksVoor, rechtsAchter);
//  delay(1000);
//  driveForwardStraight(rechtsVoor, linksVoor, rechtsAchter, linksAchter);
}
