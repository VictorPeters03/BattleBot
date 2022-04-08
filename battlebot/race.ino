
void handleRaceGame()
{
    if (status == "preparing_game")
    {
        prepareRace();
    } else if (status == "in_game")
    {
        raceGame();
    } else if (status == "finished")
    {
        stopRace();
    }
}

void prepareRace()
{
    Serial.println("prep race");
    currentJob = "race";
    logScreen();
    //Setup the IR sensors.
    pinMode(RIGHT_IR_SENSOR, INPUT);
    pinMode(LEFT_IR_SENSOR, INPUT);

    std::string str = "{\"status\": true, \"game\": \"race\"}";

    Serial.println(str.c_str());
    webSocket.sendTXT(str.c_str());
}

void raceGame()
{
    status = "in_game";
    isDriving = true;
    rightIRValue = analogRead(RIGHT_IR_SENSOR);
    leftIRValue = analogRead(LEFT_IR_SENSOR);

    //If the finish line is detected.
    if (rightIRValue < 40 && leftIRValue < 40)
    {
        stopMove();
        stopRace();
    }
        //If both sensors don't see the line go forward.
    else if (rightIRValue < 120 && leftIRValue < 120)
    {
        moveForward();
    }
        //If the right sensor sees the line.
    else if (rightIRValue > 120 && leftIRValue < 120)
    {
        stopMove();
        //delay(100);
        loopDelay = millis() + 100;
        moveRight();
        //delay(100);
    }
        //If the left sensor sees the line
    else if (rightIRValue < 120 && leftIRValue > 120)
    {
        stopMove();
        //delay(100);
        loopDelay = millis() + 100;
        moveLeft();
        //delay(100);
    }
        //If both sensors see the line.
    else if (rightIRValue > 120 && leftIRValue > 120)
    {
        moveBackwards();
        //delay(50);
        loopDelay = millis() + 50;
    }
}

void stopRace()
{
    isDriving = false;
    status = "finished";
    gameStatus = "";
    sendStatus();
    currentJob = "Waiting for command.";
    stopMove();
    //Send JSON.
}

void moveForward()
{
    analogWrite(leftFront, LOW);
    analogWrite(rightFront, LOW);
    analogWrite(leftBack, 220);
    analogWrite(rightBack, 225);
}

void moveBackwards()
{
    analogWrite(leftFront, 225);
    analogWrite(rightFront, 225);
    analogWrite(leftBack, LOW);
    analogWrite(rightBack, LOW);
}

void moveLeft()
{
    analogWrite(leftFront, LOW);
    analogWrite(rightFront, 200);
    analogWrite(leftBack, 200);
    analogWrite(rightBack, LOW);
}

void moveRight()
{
    analogWrite(leftFront, 200);
    analogWrite(rightFront, LOW);
    analogWrite(leftBack, LOW);
    analogWrite(rightBack, 200);
}

void stopMove()
{
    analogWrite(leftFront, LOW);
    analogWrite(rightFront, LOW);
    analogWrite(leftBack, LOW);
    analogWrite(rightBack, LOW);
}
