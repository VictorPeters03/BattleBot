
void handleRaceGame(const char* action){
  if(action == "prepare"){
      prepareRace();
  } else if(action == "start"){
     raceGame();
  } else if(action == "stop"){
     stopRace();
  }
}

void prepareRace(){
  status = "preparing_game";
  currentJob = "race";
  logScreen();
  //Setup the IR sensors.
  pinMode(RIGHT_IR_SENSOR, INPUT);
  pinMode(LEFT_IR_SENSOR, INPUT);
}

void raceGame() 
{
  status = "in_game";
  rightIRValue = analogRead (RIGHT_IR_SENSOR);
  leftIRValue = analogRead (LEFT_IR_SENSOR);

  /*if(rightIRValue == 0 && leftIRValue == 0){
    if(!paused){
       stopMove();
       paused = true;
       //delay(1000);
       delay = millis() + 1000;
       return;
    } else {
      paused = false;
      delay = millis() + 1000;
      return;
    }
  }
  else if(paused){
    printData(5);
    return;
  }*/
  
  //Vooruit??

  //Wit is laag
  //If both sensors don't see the line.
  if (rightIRValue < 40 && leftIRValue < 40)
  {
    stopMove();
    stopRace();
  }
  else if (rightIRValue < 120 && leftIRValue < 120)
  {
    moveForward();
  }
  //Rechts?
  //If the right sensor sees the line.
  else if (rightIRValue > 120 && leftIRValue < 120)
  {
    stopMove();
    //delay(100);
    loopDelay = millis() + 100;
    moveRight();
    //delay(100);
  }
  //Links
  //If the left sensor sees the line
  else if (rightIRValue < 120 && leftIRValue > 120)
  {
    stopMove();
    //delay(100);
    loopDelay = millis() + 100;
    moveLeft();
    //delay(100);
  }
  //Achteruit??
  //If both sensors see the line.
  else if (rightIRValue > 120 && leftIRValue > 120)
  {
    moveBackwards();
    //delay(50);
    loopDelay = millis() + 50;
  }
}
void stopRace(){
   status = "finish";
   currentJob = "Waiting for command.";
   //Send JSON.
}

void moveForward(){
    analogWrite(leftFront, LOW);
    analogWrite(rightFront, LOW);
    analogWrite(leftBack, 220);
    analogWrite(rightBack, 225);
}

void moveBackwards(){
    analogWrite(leftFront, 225);
    analogWrite(rightFront, 225);
    analogWrite(leftBack, LOW);
    analogWrite(rightBack, LOW);
}

void moveLeft(){
    analogWrite(leftFront, LOW);
    analogWrite(rightFront, 200);
    analogWrite(leftBack, 200);
    analogWrite(rightBack, LOW);
}

void moveRight(){
    analogWrite(leftFront, 200);
    analogWrite(rightFront, LOW);
    analogWrite(leftBack, LOW);
    analogWrite(rightBack, 200);
}

void stopMove(){
    analogWrite(leftFront, LOW);
    analogWrite(rightFront, LOW);
    analogWrite(leftBack, LOW);
    analogWrite(rightBack, LOW);
}
