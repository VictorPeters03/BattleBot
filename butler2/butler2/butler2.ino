#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int isStarted = 0;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

//Motor
int rightWheelForward = 17;
int rightWheelBackward = 16;
int leftWheelForward = 5;
int leftWheelBackward = 18;

//Turn duration
uint32_t duration = 2000;

//Instantiate Lidar object.
Adafruit_VL53L0X lidar = Adafruit_VL53L0X();

void drive(int rightForwardSpeed, int rightBackwardSpeed, int leftForwardSpeed, int leftBackwardSpeed)
{
  analogWrite(rightWheelForward, rightForwardSpeed);
  analogWrite(rightWheelBackward, rightBackwardSpeed);
  analogWrite(leftWheelForward, leftForwardSpeed);
  analogWrite(leftWheelBackward, leftBackwardSpeed);
}

uint16_t distance()
{
  VL53L0X_RangingMeasurementData_t measureDistance;
  lidar.rangingTest(&measureDistance, false);
  return measureDistance.RangeMilliMeter;
}

void butler() 
{
  uint16_t distances[2];  
  drive(160, LOW, 167, LOW);
  if (distance() <= 500)
  {
    drive(0, 0, 0, 0);
    uint32_t starttime = millis();
    uint32_t endtime = starttime;
    distances[0] = distance();
    distances[1] = distance();
    while((endtime - starttime) < duration)
    {
      drive(163, LOW, LOW, 166);
      distances[0] = distances[1];
      distances[1] = distance();
      endtime = millis();
      Serial.print("first distance: ");
      Serial.print(distances[0]);
      Serial.println("");
      Serial.print("second distance: ");
      Serial.print(distances[1]);
      Serial.println("");
      if (distances[0] - distances[1] > 25)
      {
        Serial.println("yes");
        for (uint32_t tStart = millis(); (millis()-tStart) < duration;)
        {
          drive(0, 0, 0, 0);
        }
      }
    while(distances[0] - distances[1] > 0 && distances[0] - distances[1] <= 30)
    {
      
    }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
    drive(183, LOW, 187, LOW);
    delay(1000);
    isStarted = 1;
  }
  butler();
}
