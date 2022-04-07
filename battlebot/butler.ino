//Instantiate Lidar object.
Adafruit_VL53L0X lidar = Adafruit_VL53L0X();
//Turn duration
uint32_t duration = 400;

long warmupTime = 0;
long loopTime = 0;

void handleButlerGame()
{
    if (status == "preparing_game")
    {
        prepareButler();
    } else if (status == "in_game")
    {
        butlerGame();
    } else if (status == "finished")
    {
        stopButler();
    }
}

void prepareButler()
{
    status = "preparing_game";
    currentJob = "butler";
    logScreen();
    //Setup the IR sensors.
    pinMode(RIGHT_IR_SENSOR, INPUT);
    pinMode(LEFT_IR_SENSOR, INPUT);

    std::string str = "{\"status\": \"true\", \"game\": \"butler\"}";   
      
    Serial.println(str.c_str());
    webSocket.sendTXT(str.c_str());
  if (!lidar.begin())
  {
    status = ("VL53L0X Error");
    stop = true;
  }
}

void butlerGame()
{
    status = "in_game";
    isDriving = true;
  if(warmupTime == 0){
    warmupTime = millis() + 400;
  } else if(millis() < warmupTime){
    drive(183, LOW, 187, LOW);
    return;
  }

    //Drive until distance between lidar and object is smaller than 30 cm.
  uint16_t distances[2];
  bool breakNextLoop = false;  
  drive(160, LOW, 165, LOW);
  if (distance() <= 350)
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
      if(handleGameLoop()){
        drive(0, 0, 0, 0);
        return;
      }
      //Look left and constantly update the lidar's measured values, while storing the previously measured value in distance[0] for comparison.
      drive(173, LOW, LOW, 176);
      distances[0] = distances[1];
      distances[1] = distance();
      endtime = millis();
      //If there is a large difference in the last and previously measured distance, the lidar has found the first part of the opening.
      if (abs(distances[0] - distances[1]) >= 80)
      {
        //The variable "breakNextLoop" ensures that the robot wont look right after having found the opening.
        breakNextLoop = true;
        //Turn left for a bit longer so that the robot will not crash into the first part of the gate.
        turnLeftTemporarily(95);

        break;
      }
    }
    starttime = millis();
    endtime = starttime;
    //This loop will be fully executed if the difference in distances was not detected in the first loop, thus meaning that the opening is on the right.
    while ((endtime - starttime) < duration * 2)
    {
      if(handleGameLoop()){
        drive(0, 0, 0, 0);
        return;
      }
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
      if (abs(distances[0] - distances[1]) >= 80)
      {
        //Turn right for a bit longer so that the robot will not crash into the first part of the gate.
        turnRightTemporarily(95);

        break;
      }
    }
  }
}

void stopButler()
{
    isDriving = false;
    status = "finish";
    currentJob = "Waiting for command.";
}


//This function makes sure the robot can drive in any direction.
void drive(int rightForwardSpeed, int rightBackwardSpeed, int leftForwardSpeed, int leftBackwardSpeed)
{
  analogWrite(rightFront, rightForwardSpeed);
  analogWrite(rightBack, rightBackwardSpeed);
  analogWrite(leftFront, leftForwardSpeed);
  analogWrite(leftBack, leftBackwardSpeed);
}

//This function makes the robot turn right for a given amount of milliseconds
void turnRightTemporarily(uint32_t milliseconds)
{
  for (uint32_t tStart = millis(); (millis()-tStart) < milliseconds;)
  {
    drive(LOW, 170, 177, LOW);
  }
}

//This function makes the robot turn left for a given amount of milliseconds
void turnLeftTemporarily(uint32_t milliseconds)
{
  for (uint32_t tStart = millis(); (millis()-tStart) < milliseconds;)
  {
    drive(173, LOW, LOW, 176);
  }
}

//This function returns the lidar's detected distance in milimeters.
uint16_t distance()
{
  VL53L0X_RangingMeasurementData_t measureDistance;
  lidar.rangingTest(&measureDistance, false);
  return measureDistance.RangeMilliMeter;
}
