
void handleMazeGame()
{
    if (status == "preparing_game")
    {
        prepareMaze();
    } else if (status == "in_game")
    {
        mazeGame();
    } else if (status == "finished")
    {
        stopMaze();
    }
}

void prepareMaze()
{
    Serial.println("prep maze");
    currentJob = "maze";
    logScreen();
    //Setup the IR sensors.
    pinMode(RIGHT_IR_SENSOR, INPUT);
    pinMode(LEFT_IR_SENSOR, INPUT);

    std::string str = "{\"status\": true, \"game\": \"maze\"}";

    Serial.println(str.c_str());
    webSocket.sendTXT(str.c_str());
}

void mazeGame()
{
  rightIRValue = analogRead (RIGHT_IR_SENSOR);
  leftIRValue = analogRead (LEFT_IR_SENSOR);

      if (warmupTime == 0)
    {
        warmupTime = millis() + 400;
    } else if (millis() < warmupTime)
    {
        drive(183, LOW, 187, LOW);
        return;
    }
  //If both infrared sensors see white, drive forward.
  if (rightIRValue < 80 && leftIRValue < 80)
  {
    drive(140, LOW, 146, LOW);
  }
  //If the right infrared sensor sees grey tape and the left infrared sensor sees white, steer to the right until both sensors see white.
  else if ((rightIRValue > 80 && rightIRValue < 1500) && leftIRValue < 75)
  {
    drive(LOW, 153, 196, LOW);
  }
  //If the left infrared sensor sees grey tape and the right infrared sensor sees white, steer to the left until both sensors see white.
  else if (rightIRValue < 75 && (leftIRValue > 80 && leftIRValue < 1500))
  {
    drive(190, LOW, LOW, 157);
  }
  /* If the right infrared sensor sees black tape, first drive forward for 100 milliseconds. 
   * If the left infrared sensor sees black tape aswell, take a sharp turn left.
   * If the left infrared sensor does not see black tape, drive forward until the right infrared sensor sees grey tape.
   * When the right infrared sensor sees grey tape, drive forward for 250 milliseconds. If the right infrared sensor sees black tape after driving, the robot will go forward.
   * If the robot does not see black tape, the robot will take a sharp turn right.
   */
  else if (rightIRValue > 1500) 
  {
    // Drive forward
    for (uint32_t tStart = millis(); (millis() - tStart) < 100;)
    {
      drive(150, LOW, 156, LOW);
    }
    // Take a sharp turn left if the left infrared sensor sees black tape
    if (leftIRValue > 1500)
    {
      for (uint32_t tStart = millis(); (millis() - tStart) < 600;)
      {
        drive(190, LOW, LOW, 197);
        if(handleGameLoop)
        {
          drive(0, 0, 0, 0);
          return;  
        }
      }
    }
    else
    {
      // Drive forward while the right infrared sensor does not see grey tape.
      while (rightIRValue > 1500)
      {
        if(handleGameLoop)
        {
          drive(0, 0, 0, 0);
          return;  
        }
        drive(140, LOW, 146, LOW);
      }
      // Drive forward for 250 milliseconds.
      for (uint32_t tStart = millis(); (millis() - tStart) < 250;)
      {
        drive(140, LOW, 146, LOW);
      }
      // Go forward if the right infrared sensor sees black tape.
      if (rightIRValue > 1500)
      {
        drive(140, LOW, 146, LOW);
      }
      else 
      {
        // Take a sharp turn right if the right sensor does not see black tape.
        for (uint32_t tStart = millis(); (millis() - tStart) < 500;)
        {
          drive(LOW, 193, 196, LOW);
        }
      }
    }
  }
  // If the left sensor detects black tape, drive forward for 500 milliseconds and then take a sharp turn left.
  else if (leftIRValue > 1500)
  {
    for (uint32_t tStart = millis(); (millis() - tStart) < 500;)
    {
        if(handleGameLoop)
        {
          drive(0, 0, 0, 0);
          return;  
        }
      drive(150, LOW, 156, LOW);
    }   
    for (uint32_t tStart = millis(); (millis() - tStart) < 600;)
    {
        if(handleGameLoop)
        {
          drive(0, 0, 0, 0);
          return;  
        }
      drive(200, LOW, LOW, 207);
    }
  }
  // If the left and the right infrared sensor detect grey tape, turn around.
  else if ((rightIRValue > 80 && rightIRValue < 1500) && (leftIRValue > 80 && leftIRValue < 1500))
  {
    for (uint32_t tStart = millis(); (millis() - tStart) < 1000;)
    {
        if(handleGameLoop)
        {
          drive(0, 0, 0, 0);
          return;  
        }
      drive(200, LOW, LOW, 207);
    }
  }
}

void stopMaze()
{
  drive(0, 0, 0, 0)
    isDriving = false;
    status = "finished";
    gameStatus = "";
    sendStatus();
    currentJob = "Waiting for command.";
    stopMove();
}
