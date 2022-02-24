#include <Arduino.h>
#include <analogWrite.h>
#include <MPU9250_asukiaaa.h>
#include <Adafruit_VL53L0X.h>

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

//adafruit vl53l0x
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void driveForwardStraight(int rechtsVoor, int linksVoor, int rechtsAchter, int linksAchter) 
{
  analogWrite(rechtsVoor, 212);
  analogWrite(linksVoor, 187);
  analogWrite(rechtsAchter, LOW);
  analogWrite(linksAchter, LOW); 
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

void driveOverTape(int sensor, int sensor2, int rechtsVoor, int linksAchter, int linksVoor, int rechtsAchter) 
{
  rightSensorValue = digitalRead (sensor);
  leftSensorValue = digitalRead (sensor2);
  if (!(rightSensorValue) && !(leftSensorValue))
  {
    analogWrite(linksVoor, LOW);
    analogWrite(rechtsVoor, LOW);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, LOW);
  }
  else if (rightSensorValue && !(leftSensorValue))
  {
    analogWrite(linksVoor, 200);
    analogWrite(rechtsVoor, 100);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, LOW);
  }
  else if (!(rightSensorValue) && leftSensorValue)
  {
    analogWrite(linksVoor, 100);
    analogWrite(rechtsVoor, 200);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, HIGH);
  }
  else 
  {
    analogWrite(linksVoor, 200);
    analogWrite(rechtsVoor, 200);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, LOW);
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
//  TCCR0B = TCCR0B & B11111000 | B00000010 ;
  pinMode(rechtsVoor, OUTPUT);
  pinMode(rechtsAchter, OUTPUT);
  pinMode(linksVoor, OUTPUT);
  pinMode(linksAchter, OUTPUT);
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
}

void loop() {
//  driveForwardStraight(rechtsVoor, linksVoor, rechtsAchter, linksAchter);
//  showTapeOutput(rightSensor, leftSensor);
//  driveOverTape(rightSensor, leftSensor, rechtsVoor, linksAchter, linksVoor, rechtsAchter);
//  delay(1000);
}
