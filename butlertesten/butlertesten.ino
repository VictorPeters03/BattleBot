#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int isStarted = 0;

int secondDistDiff = 0;

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

//void butler()
//{
//  uint16_t distances[2]; 
//  drive(160, LOW, 167, LOW);
//  Serial.print("distance: ");
//  Serial.print(distance());
//  Serial.println("");
//  if (distance() <= 200)
//  {
//    drive(0, 0, 0, 0);
//    uint32_t starttime = millis();
//    uint32_t endtime = starttime;
//    distances[0] = distance();
//    while((endtime - starttime) < duration && abs(distances[0] - distances[1]) <= 50)
//    {
//      drive(153, LOW, LOW, 156);
//      distances[0] = distances[1];
//      distances[1] = distance();
//      endtime = millis();
////      Serial.print("first distance: ");
////      Serial.print(distances[0]);
////      Serial.println("");
////      Serial.print("second distance: ");
////      Serial.print(distances[1]);
////      Serial.println("");
//    }
//    if (abs(distances[0] - distances[1]) >= 50) 
//    {
//      while (abs(distances[0] - distances[1]) < 50)
//      {
//        drive(153, LOW, LOW, 156);
//        distances[0] = distances[1];
//        distances[1] = distance();
//      }
//      while (distance() >= 50)
//      {
//        drive(160, LOW, 167, LOW);
//      }
//      
////      for (uint32_t tStart = millis(); (millis()-tStart) < duration;)
////      {
////        drive(160, LOW, 167, LOW);
////      }
//    }
//  }
//}

void butler()
{
  uint16_t distances[2];  
  drive(160, LOW, 167, LOW);
  Serial.print("distance: ");
  Serial.print(distance());
  Serial.println("");
  if (distance() <= 400)
  {
    drive(0, 0, 0, 0);
    uint32_t starttime = millis();
    uint32_t endtime = starttime;
    distances[0] = distance();
    distances[1] = distance();
    while((endtime - starttime) < duration && abs(distances[0] - distances[1]) <= 50)
    {
      drive(163, LOW, LOW, 166);
      distances[0] = distances[1];
      distances[1] = distance();
      endtime = millis();
    }
//    if (abs(distances[0] - distances[1]) >= 50) 
//    {
//      Serial.println("yesif");
//      while (abs(distances[0] - distances[1]) < 50)
//      {
//        drive(153, LOW, LOW, 156);
//        distances[1] = distance();
//        distances[0] = distances[1];
//      }
//      while (distance() >= 50)
//      {
//        drive(160, LOW, 167, LOW);
//      }
    for (uint32_t tStart = millis(); (millis()-tStart) < 10;)
    {
      drive(163, LOW, LOW, 166);
    }
//    
    uint32_t starttime2 = millis();
    uint32_t endtime2 = starttime2;
    distances[0] = distance();
    distances[1] = distance();
    while((endtime2 - starttime2) < duration && abs(distances[0] - distances[1]) <= 50)
    {
      Serial.println("yes");
      drive(163, LOW, LOW, 166);
      distances[0] = distances[1];
      distances[1] = distance();
      endtime2 = millis();
    }

    while (distance() > 60)
    {
//      Serial.println("yesdrive");
      drive(160, LOW, 167, LOW);
    }
//
    while (distance() < 500)
    {
      drive(LOW, 163, 167, LOW);
    }
//    while ((endtime - starttime) < 200)
//    {
//      drive(LOW, 163, 167, LOW);
//      endtime = millis();
//    }
         
//      for (uint32_t tStart = millis(); (millis()-tStart) < duration;)
//      {
//        drive(160, LOW, 167, LOW);
//      }
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
