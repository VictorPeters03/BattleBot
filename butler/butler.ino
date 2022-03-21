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
uint32_t duration = 800;

//Instantiate Lidar object.
Adafruit_VL53L0X lidar = Adafruit_VL53L0X();

//Adafruit SSD1306
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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

void displayDistance()
{
  VL53L0X_RangingMeasurementData_t measureDistance;

  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  if (measureDistance.RangeStatus != 4) 
  {
    display.print ("distance: ");
    display.print (measureDistance.RangeMilliMeter);
    display.println("");
  }
  display.display();
  delay(1000);
  display.clearDisplay();
}

void butler()
{
  uint16_t distances[2];  
  VL53L0X_RangingMeasurementData_t measureDistance;
  lidar.rangingTest(&measureDistance, false);
//  drive(163, LOW, 167, LOW);
//  if (measureDistance.RangeMilliMeter <= 100)
//  {
//    drive(0, 0, 0, 0);
//    delay(500);
//    for(uint32_t tStart = millis(); (millis()-tStart) < duration;)
//    {
//      drive(163, LOW, LOW, 166);
//    }
//    distances[0] = measureDistance.RangeMilliMeter;
//    for(uint32_t tStart = millis(); (millis()-tStart) < duration * 2;)
//    {
//      drive(LOW, 160, 167, LOW);
//    }
//    distances[1] = measureDistance.RangeMilliMeter;
//
//    if (distances[0] > distances[1])
//    {
//      for(uint32_t tStart = millis(); (millis()-tStart) < duration * 2;)
//      {
//        drive(163, LOW, LOW, 166);
//      }
//    }
//  }
  drive(160, LOW, 167, LOW);
  if (measureDistance.RangeMilliMeter <= 200)
  {
//    displayDistance();
    drive(0, 0, 0, 0);
    distances[0] = distance();
    for(uint32_t tStart = millis(); (millis()-tStart) < duration;)
    {
      drive(163, LOW, LOW, 166);
      distances[1] = distance();
      if (abs(distances[0] - distances[1]) >= 500)
      {
        while(abs(distances[0] - distances[1]) < 500)
        {
          drive(163, LOW, LOW, 166);
          distances[1] = distance();
          distances[0] = distances[1];
        }
      }
      else 
      {
        distances[0] = distances[1];
      }
    }
    distances[0] = distance();
    for(uint32_t tStart = millis(); (millis()-tStart) < duration * 2;)
    {
      drive(LOW, 160, 167, LOW);
    }
    distances[1] = distance();

    if (distances[0] > distances[1])
    {
      for(uint32_t tStart = millis(); (millis()-tStart) < duration * 2;)
      {
        drive(163, LOW, LOW, 166);
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
