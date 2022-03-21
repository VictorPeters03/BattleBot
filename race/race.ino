#include <Arduino.h>
#include <analogWrite.h>
//#include <MPU9250.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
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

//Adafruit SSD1306 (display)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void showOutput(int directionMode)
{
  //rightSensorValue = analogRead (rightSensorValue);
  //leftSensorValue = analogRead (leftSensorValue);
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

void moveForward(){
    showOutput(1);
    analogWrite(linksVoor, 190);
    analogWrite(rechtsVoor, 190);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, LOW);
}

void moveBackwards(){
    showOutput(4);
     //drive(160, 163, LOW, LOW);
    analogWrite(linksVoor, LOW);
    analogWrite(rechtsVoor, LOW);
    analogWrite(linksAchter, 190);
    analogWrite(rechtsAchter, 190);
}

void moveLeft(){
    showOutput(3);
    //drive(LOW, 193, 196, LOW);
    analogWrite(linksVoor, LOW);
    analogWrite(rechtsVoor, 190);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, LOW);
}

void moveRight(){
    showOutput(2);
    //drive(190, LOW, LOW, 197);
    analogWrite(linksVoor, 190);
    analogWrite(rechtsVoor, LOW);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, LOW);
}

void race(int sensor, int sensor2, int rechtsVoor, int linksAchter, int linksVoor, int rechtsAchter) 
{
  rightSensorValue = analogRead (sensor);
  leftSensorValue = analogRead (sensor2);
  //Vooruit??
  if (rightSensorValue > 120 && leftSensorValue > 120)
  {
    moveForward();
  }
  //Rechts?
  else if (rightSensorValue < 120 && leftSensorValue > 120)
  {
    moveRight();
  }
  //Links
  else if (rightSensorValue > 120 && leftSensorValue > 120)
  {
    moveLeft();
  }
  //Achteruit??
  else if (rightSensorValue < 120 && leftSensorValue < 120) 
  {
    moveBackwards();
  }
}

void setup() {
  //Setup the motors
  pinMode(rechtsVoor, OUTPUT);
  pinMode(rechtsAchter, OUTPUT);
  pinMode(linksVoor, OUTPUT);
  pinMode(linksAchter, OUTPUT);
  //Set all motors to 0 so they stop.
  analogWrite(linksVoor, 0);
  analogWrite(rechtsVoor, 0);
  analogWrite(linksAchter, 0);
  analogWrite(rechtsAchter, 0);
  //Setup the IR sensors.
  pinMode(rightSensor, INPUT);
  pinMode(leftSensor, INPUT);

  //Serial.begin (115200);

  //Start serial monitor
  /*while (!Serial) 
  {
    delay(1);
  }*/

  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.display();
  
  delay(2000);
  display.clearDisplay();
  display.display();
}

void loop() {
  if (!isStarted)
  {
    delay(1000);
    isStarted = 1;
  }
  race(rightSensor, leftSensor, rechtsVoor, linksAchter, linksVoor, rechtsAchter);
}
