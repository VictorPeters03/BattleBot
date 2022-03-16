#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>

//Motor
int rightWheelForward = 17;
int rightWheelBackward = 16;
int leftWheelForward = 5;
int leftWheelBackward = 18;

//Turn duration
uint32_t duration = 10000;

//Instantiate Lidar object.
Adafruit_VL53L0X lidar = Adafruit_VL53L0X();

void drive(int rightForwardSpeed, int rightBackwardSpeed, int leftForwardSpeed, int leftBackwardSpeed)
{
  analogWrite(rightWheelForward, rightForwardSpeed);
  analogWrite(rightWheelBackward, rightBackwardSpeed);
  analogWrite(leftWheelForward, leftForwardSpeed);
  analogWrite(leftWheelBackward, leftBackwardSpeed);
}

void butler()
{
  VL53L0X_RangingMeasurementData_t measureDistance;
  drive(123, 120, LOW, LOW);
  if (measureDistance.RangeMillimeter <= 100)
  {
    drive(0, 0, 0, 0);
    delay(500);
    for(uint32_t tStart = millis(); (millis()-tStart) < duration;)
    {
      if(measureDistance.RangeMillimeter >= 210)
      {
        drive(123, 0, 127, 0);
      }
      else
      {
        drive(123, 0, 0, 126);
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(rightWheelForward, OUTPUT);
  pinMode(rightWheelBackward, OUTPUT);
  pinMode(leftWheelForward, OUTPUT);
  pinMode(leftWheelBackward, OUTPUT);
  drive(0, 0, 0, 0);

  //check if VL53L0X starts up
  if (!lidar.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!isStarted)
  {
    drive(183, 180, LOW, LOW);
    delay(1000);
    isStarted = 1;
  }

}
