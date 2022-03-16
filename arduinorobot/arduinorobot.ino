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

int engineSpeed = 170;

//adafruit vl53l0x
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

//Adafruit SSD1306
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void driveForwardStraight(int rechtsVoor, int linksVoor, int rechtsAchter, int linksAchter) 
{
  analogWrite(rechtsVoor, LOW);
  analogWrite(linksVoor, LOW);
  analogWrite(rechtsAchter, 187);
  analogWrite(linksAchter, 196); 
}

void showTapeOutput(int sensor, int sensor2)
{
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  rightSensorValue = analogRead (sensor);
  leftSensorValue = analogRead (sensor2);
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


//Deze functie is voor de robot wanneer bij deze de twee dikke wielen vooraan staan.
//void driveOverTape(int sensor, int sensor2, int rechtsVoor, int linksAchter, int linksVoor, int rechtsAchter) 
//{
//  rightSensorValue = digitalRead (sensor);
//  leftSensorValue = digitalRead (sensor2);
//  if (!(rightSensorValue) && !(leftSensorValue))
//  {
//    analogWrite(linksVoor, 180);
//    analogWrite(rechtsVoor, 180);
//    analogWrite(linksAchter, LOW);
//    analogWrite(rechtsAchter, LOW);
//  }
//  else if (rightSensorValue && !(leftSensorValue))
//  {
//    analogWrite(linksVoor, 180);
//    analogWrite(rechtsVoor, LOW);
//    analogWrite(linksAchter, LOW);
//    analogWrite(rechtsAchter, 180);
//  }
//  else if (!(rightSensorValue) && leftSensorValue)
//  {
//    analogWrite(linksVoor, LOW);
//    analogWrite(rechtsVoor, 180);
//    analogWrite(linksAchter, 180);
//    analogWrite(rechtsAchter, LOW);
//  }
//  else 
//  {
//    analogWrite(linksVoor, 180);
//    analogWrite(rechtsVoor, 180);
//    analogWrite(linksAchter, LOW);
//    analogWrite(rechtsAchter, LOW);
//  }
//}

//Deze volgende 2 functies zijn voor de robot wanneer bij deze de twee dikke wielen achteraan staan.
//void driveOverTape(int sensor, int sensor2, int rechtsVoor, int linksAchter, int linksVoor, int rechtsAchter) 
//{
//  rightSensorValue = digitalRead (sensor);
//  leftSensorValue = digitalRead (sensor2);
//  if (!(rightSensorValue) && !(leftSensorValue))
//  {
//    analogWrite(linksVoor, 160);
//    analogWrite(rechtsVoor, 163);
//    analogWrite(linksAchter, LOW);
//    analogWrite(rechtsAchter, LOW);
//  }
//  else if (rightSensorValue && !(leftSensorValue))
//  {
//    analogWrite(linksVoor, LOW);
//    analogWrite(rechtsVoor, LOW);
//    analogWrite(linksAchter, 148);
//    analogWrite(rechtsAchter, LOW);
//  }
//  else if (!(rightSensorValue) && leftSensorValue)
//  {
//    analogWrite(linksVoor, LOW);
//    analogWrite(rechtsVoor, LOW);
//    analogWrite(linksAchter, LOW);
//    analogWrite(rechtsAchter, 143);
//  }
//  else 
//  {
//    analogWrite(linksVoor, LOW);
//    analogWrite(rechtsVoor, LOW);
//    analogWrite(linksAchter, 148);
//    analogWrite(rechtsAchter, 143);
//  }
//}

void drive(int linksVoorSnelheid, int rechtsVoorSnelheid, int linksAchterSnelheid, int rechtsAchterSnelheid)
{
  analogWrite(linksVoor, linksVoorSnelheid);
  analogWrite(rechtsVoor, rechtsVoorSnelheid);
  analogWrite(linksAchter, linksAchterSnelheid);
  analogWrite(rechtsAchter, rechtsAchterSnelheid);
}

void brake(int rightSensorValue, int leftSensorValue)
{
  while(engineSpeed > 0)
  {
    engineSpeed -= 50;
  }
  while(rightSensorValue < 80 && leftSensorValue < 80)
  {
    analogWrite(forwardLeft, LOW);
    analogWrite(forwardRight, LOW);
    analogWrite(reverseLeft, 170);
    analogWrite(reverseRight, 170);
  }
}

void driveOverTape(int sensor, int sensor2, int rechtsVoor, int linksAchter, int linksVoor, int rechtsAchter) 
{
  rightSensorValue = analogRead (sensor);
  leftSensorValue = analogRead (sensor2);
  if (rightSensorValue < 500 && leftSensorValue < 500)
  {
    drive(LOW, LOW, 176, 167);
  }
  else if (rightSensorValue > 1500 && leftSensorValue < 500)
  {
    drive(LOW, 193, 196, LOW);
  }
  else if (rightSensorValue < 500 && leftSensorValue > 1500)
  {
    drive(190, LOW, LOW, 197);
  }
  else if (rightSensorValue > 1500 && leftSensorValue > 1500) 
  {
    drive(160, 163, LOW, LOW);
  }
}

void maze(int sensor, int sensor2, int rechtsVoor, int linksAchter, int linksVoor, int rechtsAchter)
{
  rightSensorValue = analogRead (sensor);
  leftSensorValue = analogRead (sensor2);
  if (rightSensorValue < 80 && leftSensorValue < 80)
  {
    drive(LOW, LOW, 206, 197);
  }
  else if (rightSensorValue > 90 && leftSensorValue > 90)
  {
    brake();
  }
//  else if (rightSensorValue > 1500 && leftSensorValue < 500)
//  {
//    drive(LOW, LOW, 206, 157);
//  }
//  else if (rightSensorValue < 500 && leftSensorValue > 1500)
//  {
//    drive(LOW, LOW, 166, 197);
//  }
//  else if (rightSensorValue > 1500 && leftSensorValue > 1500) 
//  {
//    drive(160, 163, LOW, LOW);
//  }
}

//Deze functie meet de afstand tussen de afstandsensor en een object.
void displayDistance()
{
  VL53L0X_RangingMeasurementData_t measureDistance;

  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  if (measureDistance.RangeStatus != 4) {
    display.print ("distance: ");
    display.print (measureDistance.RangeMilliMeter);
    display.println("");
  }
  display.display();
  delay(1000);
  display.clearDisplay();
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
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  //check if VL53L0X starts up
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  delay(2000);
}

void loop() {
  if (!isStarted)
  {
    driveForwardStraight(rechtsVoor, linksVoor, rechtsAchter, linksAchter);
    delay(1000);
    isStarted = 1;
  }
//  showTapeOutput(rightSensor, leftSensor);
//  driveOverTape(rightSensor, leftSensor, rechtsVoor, linksAchter, linksVoor, rechtsAchter);
  maze(rightSensor, leftSensor, rechtsVoor, linksAchter, linksVoor, rechtsAchter);
//  delay(1000);
//  driveForwardStraight(rechtsVoor, linksVoor, rechtsAchter, linksAchter);
}
