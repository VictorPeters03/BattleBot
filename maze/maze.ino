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

void maze()
{
  rightSensorValue = analogRead (rightSensor);
  leftSensorValue = analogRead (leftSensor);
  //If both infrared sensors see white, drive forward.
  if (rightSensorValue < 80 && leftSensorValue < 80)
  {
    drive(140, LOW, 146, LOW);
  }
  //If the right infrared sensor sees grey tape and the left infrared sensor sees white, steer to the right until both sensors see white.
  else if ((rightSensorValue > 80 && rightSensorValue < 1500) && leftSensorValue < 75)
  {
    drive(LOW, 153, 196, LOW);
  }
  //If the left infrared sensor sees grey tape and the right infrared sensor sees white, steer to the left until both sensors see white.
  else if (rightSensorValue < 75 && (leftSensorValue > 80 && leftSensorValue < 1500))
  {
    drive(190, LOW, LOW, 157);
  }
  /* If the right infrared sensor sees black tape, first drive forward for 100 milliseconds. 
   * If the left infrared sensor sees black tape aswell, take a sharp turn left.
   * If the left infrared sensor does not see black tape, drive forward until the right infrared sensor sees grey tape.
   * When the right infrared sensor sees grey tape, drive forward for 250 milliseconds. If the right infrared sensor sees black tape after driving, the robot will go forward.
   * If the robot does not see black tape, the robot will take a sharp turn right.
   */
  else if (rightSensorValue > 1500) 
  {
    // Drive forward
    for (uint32_t tStart = millis(); (millis() - tStart) < 100;)
    {
      drive(150, LOW, 156, LOW);
    }
    // Take a sharp turn left if the left infrared sensor sees black tape
    if (leftSensorValue > 1500)
    {
      for (uint32_t tStart = millis(); (millis() - tStart) < 600;)
      {
        drive(190, LOW, LOW, 197);
      }
    }
    else
    {
      // Drive forward while the right infrared sensor does not see grey tape.
      while (rightSensorValue > 1500)
      {
        drive(140, LOW, 146, LOW);
      }
      // Drive forward for 250 milliseconds.
      for (uint32_t tStart = millis(); (millis() - tStart) < 250;)
      {
        drive(140, LOW, 146, LOW);
      }
      // Go forward if the right infrared sensor sees black tape.
      if (rightSensorValue > 1500)
      {
        drive(140, LOW, 146, LOW);
      }
      else 
      {
        // Take a sharp turn right if the right sensor does not see black tape.
        for (uint32_t tStart = millis(); (millis() - tStart) < 500)
        {
          drive(LOW, 193, 196, LOW);
        }
      }
    }
  }
  // If the left sensor detects black tape, drive forward for 500 milliseconds and then take a sharp turn left.
  else if (leftSensorValue > 1500)
  {
    for (uint32_t tStart = millis(); (millis() - tStart) < 500;)
    {
      drive(150, LOW, 156, LOW);
    }   
    for (uint32_t tStart = millis(); (millis() - tStart) < 600;)
    {
      drive(200, LOW, LOW, 207);
    }
  }
  // If the left and the right infrared sensor detect grey tape, turn around.
  else if ((rightSensorValue > 80 && rightSensorValue < 1500) && (leftSensorValue > 80 && leftSensorValue < 1500))
  {
    for (uint32_t tStart = millis(); (millis() - tStart) < 1000;)
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
  maze();
}
