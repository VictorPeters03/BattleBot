
void handleButlerGame(const char* action){
  if(action == "prepare"){
      prepareRace();
  } else if(action == "start"){
     raceGame();
  } else if(action == "stop"){
     stopRace();
  }
}

void prepareButler(){
  status = "preparing_game";
  currentJob = "race";
  logScreen();
  //Setup the IR sensors.
  pinMode(RIGHT_IR_SENSOR, INPUT);
  pinMode(LEFT_IR_SENSOR, INPUT);
}

void butlerGame() 
{
}

void finishButlerGame(){
}
