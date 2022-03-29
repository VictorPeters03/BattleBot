#include <Arduino.h>
#include <analogWrite.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

bool isStarted = false;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

//Motor
int rightWheelForward = 17;
int rightWheelBackward = 16;
int leftWheelForward = 5;
int leftWheelBackward = 18;

//Turn duration
uint32_t duration = 600;

//Adafruit SSD1306 (display)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Instantiate Lidar object.
Adafruit_VL53L0X lidar = Adafruit_VL53L0X();

//This function makes sure the robot can drive in any direction.
void drive(int rightForwardSpeed, int rightBackwardSpeed, int leftForwardSpeed, int leftBackwardSpeed)
{
  analogWrite(rightWheelForward, rightForwardSpeed);
  analogWrite(rightWheelBackward, rightBackwardSpeed);
  analogWrite(leftWheelForward, leftForwardSpeed);
  analogWrite(leftWheelBackward, leftBackwardSpeed);
}

//This function can print data on the Adafruit SSD1306
void printData(uint16_t distances[], uint32_t starttime, uint32_t endtime, boolean yes)
{
  //rightSensorValue = analogRead (rightSensorValue);
  //leftSensorValue = analogRead (leftSensorValue);
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print ("D: 0:");
  display.print (distances[0]);
  display.print (", 1: ");
  display.print (distances[1]);
  display.println("");
  display.print ("2: ");
  display.print (distances[2]);
  if(yes){
    display.print (" - YES!!");
  }

  display.println("");
  //display.print("INF1C Robot");
  display.print("Time: ");
  display.print(starttime);
  display.print(" - ");
  display.print(endtime);
  display.display();
  
  //display.clearDisplay();
}

//This function returns the lidar's detected distance in milimeters.
uint16_t distance()
{
  VL53L0X_RangingMeasurementData_t measureDistance;
  lidar.rangingTest(&measureDistance, false);
  return measureDistance.RangeMilliMeter;
}

//This function executes the butler game.
void butler() 
{
  //Drive until distance between lidar and object is smaller than 30 cm.
  uint16_t distances[2];
  bool breakNextLoop = false;  
  drive(160, LOW, 165, LOW);
  if (distance() <= 450)
  {
    //Stand still and store the measured distance in distance[0] and distance[1].
    drive(0, 0, 0, 0);
    uint32_t starttime = millis();
    uint32_t endtime = starttime;
    distances[0] = distance();
    distances[1] = distance();
    //Execute this loop for 800 ms.
    while ((endtime - starttime) < duration)
    {
      //Look left and constantly update the lidar's measured values, while storing the previously measured value in distance[0] for comparison.
      drive(170, LOW, LOW, 173);
      distances[0] = distances[1];
      distances[1] = distance();
      endtime = millis();
      //If there is a large difference in the last and previously measured distance, the lidar has found the first part of the opening.
      if (abs(distances[0] - distances[1]) >= 70)
      {
        //The variable "breakNextLoop" ensures that the robot wont look right after having found the opening.
        breakNextLoop = true;
        //Turn left for a bit longer so that the robot will not crash into the first part of the gate.
        for (uint32_t tStart = millis(); (millis()-tStart) < 230;)
        {
          drive (163, LOW, LOW, 166);

        }
        drive(0, 0, 0, 0);
        //Drive towards the gate until the robot is 3 cm's or less from the wall.
        while (distance() > 230)
        {
          drive(160, LOW, 167, LOW);
        }
        drive(0, 0, 0, 0);
        //Turn right until the robot is able to go through the opening.
        distances[0] = distance();
        distances[1] = distance();
        while (distance() < 550)
        {
          drive(LOW, 160, 167, LOW);
          distances[0] = distances[1];
          distances[1] = distance();
        }
        for (uint32_t tStart = millis(); (millis()-tStart) < 50;)
        {
          drive (LOW, 160, 167, LOW);
        }
        drive(0, 0, 0, 0);
        break;
      }
    }
    starttime = millis();
    endtime = starttime;
    //This loop will be fully executed if the difference in distances was not detected in the first loop, thus meaning that the opening is on the right.
    while ((endtime - starttime) < duration * 2)
    {
      //The loop will not execute is "breakNextLoop" is true.
      if (breakNextLoop)
      {
        break;
      }
      //Look right and constantly update the lidar's measured values, while storing the previously measured value in distance[0] for comparison.
      drive(LOW, 170, 177, LOW);
      distances[0] = distances[1];
      distances[1] = distance();
      endtime = millis();
      //If there is a large difference in the last and previously measured distance, the lidar has found the first part of the opening.
      if (abs(distances[0] - distances[1]) >= 70)
      {
        //Turn right for a bit longer so that the robot will not crash into the first part of the gate.
        for (uint32_t tStart = millis(); (millis()-tStart) < 230;)
        {
          drive (LOW, 160, 167, LOW);
        }
        drive(0, 0, 0, 0);
        //Drive towards the gate until the robot is 3 cm's or less from the wall.
        while (distance() > 230)
        {
          drive(160, LOW, 167, LOW);
        }
        drive(0, 0, 0, 0);
        //Turn left until the robot is able to go through the opening.
        distances[0] = distance();
        distances[1] = distance();
        while (distance() < 550)
        {
          drive(163, LOW, LOW, 166);
          distances[0] = distances[1];
          distances[1] = distance();
        }
        drive(0, 0, 0, 0);
        for (uint32_t tStart = millis(); (millis()-tStart) < 50;)
        {
          drive (163, LOW, LOW, 166);
        }
        drive(0, 0, 0, 0);
        break;
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
  for (uint32_t tStart = millis(); (millis()-tStart) < 2000;);
}

void loop() {
  /*The following if statement will start the motors for 400 milliseconds at the beginning of each program.
   *This statement will be ran once.
   */
  if (!isStarted)
  {
    for (uint32_t tStart = millis(); (millis()-tStart) < 400;)
    {
      drive(183, LOW, 187, LOW);
    }
    isStarted = true;
  }
  butler();
}
