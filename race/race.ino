#include <Arduino.h>
#include <analogWrite.h>
#include <MPU9250.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Motor
int rechtsVoor = 17; //Dit is linksachter bij de tweede driveOverTape functie
int linksAchter = 18; //Dit is rechtsvoor bij de tweede driveOverTape functie
int linksVoor = 5; //Dit is rechtsachter bij de tweede driveOverTape functie
int rechtsAchter = 16; //Dit is linksvoor bij de tweede driveOverTape functie
int isStarted = 0; //Deze gaat op 1 wanneer de robot 1 seconde lang gestart is

// Zwart wit sensor
int rightSensor = 39; //Dit is de linkersensor bij driveOverTape 2 en 3 
int leftSensor = 34; //Dit is de rechtersensor bij driveOverTape 2 en 3
int rightSensorValue = 0;
int leftSensorValue = 0; 

//adafruit vl53l0x
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
//VL53L0X_RangingMeasurementData_t measureDistance;

//Adafruit SSD1306
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/*void driveForwardStraight(int rechtsVoor, int linksVoor, int rechtsAchter, int linksAchter) 
{
  analogWrite(rechtsVoor, LOW);
  analogWrite(linksVoor, LOW);
  analogWrite(rechtsAchter, 187);
  analogWrite(linksAchter, 196); 
}*/

void drive(int linksVoorSnelheid, int rechtsVoorSnelheid, int linksAchterSnelheid, int rechtsAchterSnelheid)
{
  analogWrite(linksVoor, linksVoorSnelheid);
  analogWrite(rechtsVoor, rechtsVoorSnelheid);
  analogWrite(linksAchter, linksAchterSnelheid);
  analogWrite(rechtsAchter, rechtsAchterSnelheid);
}

void showOutput(int sensor, int sensor2, int directionMode)
{
  rightSensorValue = analogRead (sensor);
  leftSensorValue = analogRead (sensor2);
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print ("LR sensors: ");
  display.print (leftSensorValue);
  display.print("/");
  display.print(rightSensorValue);
  display.println("");
  display.print ("Mode: ");
  if(directionMode == 1){
      display.print ("vooruit (1)");
  } else if(directionMode == 2){
      display.print ("rechts (2)");
  } else if(directionMode == 3){
      display.print ("links (3)");
  } else if(directionMode == 4){
      display.print ("achteruit (4)");
  }
  display.println("");
  display.print("INF1C Robot");
  display.display();
  delay(100);

  //display.clearDisplay();
}

void race(int sensor, int sensor2, int rechtsVoor, int linksAchter, int linksVoor, int rechtsAchter) 
{
  /*if (measureDistance.RangeStatus != 4) {
    //Noodstop
    if(measureDistance.RangeMilliMeter < 100){
      return;
    }
  }*/
  rightSensorValue = analogRead (sensor);
  leftSensorValue = analogRead (sensor2);
  //Vooruit??
  if (rightSensorValue > 120 && leftSensorValue > 120)
  {
    showOutput(sensor, sensor2, 1);
    //drive(LOW, LOW, 176, 167);
    drive(190, 190, LOW, LOW);
  }
  //Rechts?
  else if (rightSensorValue < 120 && leftSensorValue > 120)
  {
    showOutput(sensor, sensor2, 2);
    
    //drive(LOW, 193, 196, LOW);
    drive(LOW, 190, LOW, LOW);
  }
  //Links
  else if (rightSensorValue > 120 && leftSensorValue > 120)
  {
    showOutput(sensor, sensor2, 3);
    //drive(190, LOW, LOW, 197);
    drive(190, LOW, LOW, LOW);
  }
  //Achteruit??
  else if (rightSensorValue < 120 && leftSensorValue < 120) 
  {
    showOutput(sensor, sensor2, 4);
     //drive(160, 163, LOW, LOW);
    drive(LOW, LOW, 190, 190);
  }
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

  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.display();

  //check if VL53L0X starts up
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  delay(2000);
  display.clearDisplay();
  display.display();
}

void loop() {
  if (!isStarted)
  {
    //driveForwardStraight(rechtsVoor, linksVoor, rechtsAchter, linksAchter);
    delay(1000);
    isStarted = 1;
  }
//  showTapeOutput(rightSensor, leftSensor);
//  driveOverTape(rightSensor, leftSensor, rechtsVoor, linksAchter, linksVoor, rechtsAchter);
  race(rightSensor, leftSensor, rechtsVoor, linksAchter, linksVoor, rechtsAchter);
//  delay(1000);
//  driveForwardStraight(rechtsVoor, linksVoor, rechtsAchter, linksAchter);
}
