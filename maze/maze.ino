#include <Arduino.h>
#include <analogWrite.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

bool isStarted = false;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

//Motor
int rightWheelForward = 18;
int rightWheelBackward = 5;
int leftWheelForward = 16;
int leftWheelBackward = 17;

// Zwart wit sensor
int rightSensor = 34; // Right infrared sensor (MH sensor series) 
int leftSensor = 39; // Left infrared sensor (MH sensor series) 
int rightSensorValue = 0;
int leftSensorValue = 0; 

//Adafruit SSD1306 (display)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//This function makes sure the robot can drive in any direction.
void drive(int rightForwardSpeed, int rightBackwardSpeed, int leftForwardSpeed, int leftBackwardSpeed)
{
  analogWrite(rightWheelForward, rightForwardSpeed);
  analogWrite(rightWheelBackward, rightBackwardSpeed);
  analogWrite(leftWheelForward, leftForwardSpeed);
  analogWrite(leftWheelBackward, leftBackwardSpeed);
}

void showTapeOutput()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  rightSensorValue = analogRead (rightSensor);
  leftSensorValue = analogRead (leftSensor);
  display.print ("Right sensor: ");
  display.print (rightSensorValue);
  display.println("");
  display.print ("Left sensor: ");
  display.print (leftSensorValue);
  display.println("");
  display.display();
  delay(1000);
  display.clearDisplay();
}

void maze(int rightSensor, int leftSensor)
{
  rightSensorValue = analogRead (rightSensor);
  leftSensorValue = analogRead (leftSensor);
  if (rightSensorValue < 80 && leftSensorValue < 80)
  {
    drive(150, LOW, 156, LOW);
  }
  else if ((rightSensorValue > 80 && rightSensorValue < 1500) && leftSensorValue < 75)
  {
    drive(LOW, 163, 206, LOW);
  }
  else if (rightSensorValue < 75 && (leftSensorValue > 80 && leftSensorValue < 1500))
  {
    drive(200, LOW, LOW, 167);
  }
  else if (rightSensorValue > 1500) 
  {
    for (uint32_t tStart = millis(); (millis() - tStart) < 600;)
    {
      drive(150, LOW, 156, LOW);
    }
    if (rightSensorValue < 2000)
    {
      for (uint32_t tStart = millis(); (millis() - tStart) < 600;)
      {
        drive(LOW, 203, 206, LOW);
      }
    }
    
//    if (rightSensorValue < 2250)
//    {
//      for (uint32_t tStart = millis(); (millis() - tStart) < 600;)
//      {
//        drive(LOW, 203, 206, LOW);
//      }
//        while (leftSensorValue < 50)
//        {
//          drive(LOW, 163, 166, LOW);
//        }
//        for (uint32_t tStart = millis(); (millis() - tStart) < 400;)
//        {
//          drive(LOW, 203, 206, LOW);
//        }
    }
  else if (leftSensorValue > 1500)
  {
    for (uint32_t tStart = millis(); (millis() - tStart) < 500;)
    {
      drive(150, LOW, 156, LOW);
    }
//    for (uint32_t tStart = millis(); (millis() - tStart) < 400;)
//    {
//      drive(200, LOW, LOW, 207);
//    }
//    while (rightSensorValue < 50)
//    {
//      drive(160, LOW, LOW, 167);
//    }
    
    for (uint32_t tStart = millis(); (millis() - tStart) < 600;)
    {
      drive(200, LOW, LOW, 207);
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
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!isStarted)
  {
    for (uint32_t tStart = millis(); (millis()-tStart) < 400;)
    {
      drive(183, LOW, 187, LOW);
    }
    isStarted = true;
  }
  maze(rightSensor, leftSensor);
//showTapeOutput();
}
