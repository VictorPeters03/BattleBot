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

uint16_t distances[3]; 
uint32_t starttime = millis();
uint32_t endtime = starttime;

//Turn duration
uint32_t duration = 2000;

//Adafruit SSD1306 (display)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Instantiate Lidar object.
Adafruit_VL53L0X lidar = Adafruit_VL53L0X();

void drive(int rightForwardSpeed, int rightBackwardSpeed, int leftForwardSpeed, int leftBackwardSpeed)
{
  analogWrite(rightWheelForward, rightForwardSpeed);
  analogWrite(rightWheelBackward, rightBackwardSpeed);
  analogWrite(leftWheelForward, leftForwardSpeed);
  analogWrite(leftWheelBackward, leftBackwardSpeed);
}

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

uint16_t distance()
{
  VL53L0X_RangingMeasurementData_t measureDistance;
  lidar.rangingTest(&measureDistance, false);
  return measureDistance.RangeMilliMeter;
}

void butler() 
{
  uint16_t debugDistances[2];
  debugDistances[0] = distances[0];
  debugDistances[1] = distances[1];
  debugDistances[2] = distance();
  printData(debugDistances, starttime, endtime, false);
      
  drive(160, LOW, 167, LOW);
  if (distance() <= 500)
  {
    drive(0, 0, 0, 0);
    starttime = millis();
    endtime = starttime;
    distances[0] = distance();
    distances[1] = distance();
    //For debug.
    distances[2] = distance();
    printData(distances, starttime, endtime, false);
    while((endtime - starttime) < duration)
    {
      drive(163, LOW, LOW, 166);
      distances[0] = distances[1];
      distances[1] = distance();
      distances[2] = distance();
      endtime = millis();
      printData(distances, starttime, endtime, false);
      Serial.print("first distance: ");
      Serial.print(distances[0]);
      Serial.println("");
      Serial.print("second distance: ");
      Serial.print(distances[1]);
      Serial.println("");
      if (distances[0] - distances[1] > 25)
      {
        printData(distances, starttime, endtime, true);
        Serial.println("yes");
        for (uint32_t tStart = millis(); (millis()-tStart) < duration;)
        {
          drive(0, 0, 0, 0);
        }
      }
    while(distances[0] - distances[1] > 0 && distances[0] - distances[1] <= 30)
    {
      debugDistances[0] = distances[0];
      debugDistances[1] = distances[1];
      debugDistances[2] = distance();
      printData(debugDistances, starttime, endtime, false);
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
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.display();
  
  delay(2000);
  display.clearDisplay();
  display.display();
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
