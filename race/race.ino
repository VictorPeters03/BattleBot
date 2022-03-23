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

boolean paused = false;

//Adafruit SSD1306 (display)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void printData(int directionMode)
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
  } else if(directionMode == 5){
      display.print ("stopped (5)");
  }
  display.println("");
  display.print("INF1C Robot");
  display.display();
  
  //display.clearDisplay();
}

void moveForward(){
    printData(1);
    analogWrite(linksVoor, LOW);
    analogWrite(rechtsVoor, LOW);
    analogWrite(linksAchter, 222);
    analogWrite(rechtsAchter, 225);
}

void moveBackwards(){
    printData(4);
     //drive(160, 163, LOW, LOW);
    analogWrite(linksVoor, 222);
    analogWrite(rechtsVoor, 225);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, LOW);
}

void moveLeft(){
    printData(3);
    //drive(LOW, 193, 196, LOW);
    analogWrite(linksVoor, LOW);
    analogWrite(rechtsVoor, 225);
    analogWrite(linksAchter, 225);
    analogWrite(rechtsAchter, LOW);
}

void moveRight(){
    printData(2);
    //drive(190, LOW, LOW, 197);
    analogWrite(linksVoor, 225);
    analogWrite(rechtsVoor, LOW);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, 225);
}

void stopMove(){
    printData(5);
    analogWrite(linksVoor, LOW);
    analogWrite(rechtsVoor, LOW);
    analogWrite(linksAchter, LOW);
    analogWrite(rechtsAchter, LOW);
}

void race(int sensor, int sensor2, int rechtsVoor, int linksAchter, int linksVoor, int rechtsAchter) 
{
  rightSensorValue = analogRead (sensor);
  leftSensorValue = analogRead (sensor2);

  if(rightSensorValue == 0 && leftSensorValue == 0){
    if(!paused){
       stopMove();
       paused = true;
       delay(1000);
       return;
    } else {
      paused = false;
      delay(1000);
    }
  }
  else if(paused){
    printData(5);
    return;
  }
  
  //Vooruit??

  //Wit is laag
  //If both sensors don't see the line.
  if (rightSensorValue < 1000 && leftSensorValue < 1000)
  {
    moveForward();
  }
  //Rechts?
  //If the right sensor sees the line.
  else if (rightSensorValue > 1000 && leftSensorValue < 1000)
  {
    stopMove();
    delay(100);
    moveRight();
    delay(100);
  }
  //Links
  //If the left sensor sees the line
  else if (rightSensorValue < 1000 && leftSensorValue > 1000)
  {
    stopMove();
    delay(100);
    moveLeft();
    delay(100);
  }
  //Achteruit??
  //If both sensors see the line.
  else if (rightSensorValue > 1000 && leftSensorValue > 1000)
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
