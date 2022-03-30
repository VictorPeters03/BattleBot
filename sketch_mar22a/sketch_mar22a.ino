//Websocket client
//Datum: 22 maart
//seri data naar json
//deseri json naar data

//doc.clear();

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

//Connecting to WiFi.
const char* ssid = "battlebot_nl";
const char* password = "battlebot_nl";

//Network data, Interface group.
const char host[] = "10.110.111.103";
const char path[] = "/";
const int port = 3003;

WiFiClient client;
WebSocketsClient webSocket;

void setup() {
  Serial.begin(9600);
  //sendLoginRequest();

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Pending connection...");
    delay(500);
} 
  Serial.println("You successfully connected with the network.");
  Serial.println("The IP-Adress is: ");
  Serial.println(WiFi.localIP().toString());
  Serial.println("The MAC-Address is: ");
  Serial.println(WiFi.macAddress());
  
  // server address, port and URL
  webSocket.begin(host, port, path);

  //Event handler
  webSocket.onEvent(webSocketEvent);
  
  //Tries to reconnect every 5 seconds. 
  webSocket.setReconnectInterval(5000);
}

//This causes the robot to send a login authentication request to the server.
void sendLoginRequest(){
   
  Serial.println("Posting JSON data to the server...");
  StaticJsonDocument<100> doc;

  doc["action"] = "login";
  doc["id"] = "84:CC:A8:7A:A2:A8";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}

//If the above login request failed, the server will tell the robot about it
void faultyLogin() {
  StaticJsonDocument<50> doc;

  char json[] = "{\"loggedin\":\"false\"}";

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  bool loggedin = doc["loggedin"];
  Serial.println(loggedin);
}

//If it was successfull the server will tell the robot about it too
void loginSuccess() {
  StaticJsonDocument<50> doc;

  char json[] = "{\"loggedin\":\"true\"}";

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  bool loggedin = doc["loggedin"];
  Serial.println(loggedin);
}

//This causes the robot to prepare for the maze game
void prepareGameMaze() {
  StaticJsonDocument<100> doc;

  char json[] = "{\"action\":\"prepare\"game\":\"maze\"}";

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char action = doc["action"];
  const char game = doc["game"];
  Serial.println(action);
  Serial.println(game);
}

//This causes the robot to prepare for the Butler game.
void prepareGameButler(){
  StaticJsonDocument<100> doc;

  char json[] = "{\"action\":\"prepare\"game\":\"butler\"}";

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char action = doc["action"];
  const char game = doc["game"];
  Serial.println(action);
  Serial.println(game);
}

//Let's the server know that it's ready to play the maze game
void startGameMaze(){
  
  Serial.println("Ready to play the game...");
  StaticJsonDocument<65> doc;

  doc["status"] = "true";
  doc["game"] = "maze";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}

//Tells the server that the robot can not start the game
void cantStartGameMaze(){
  
  Serial.println("Unable to play the current game...");
  StaticJsonDocument<50> doc;

  doc["status"] = "false";
  doc["game"] = "maze";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}
//Let's the server know that it's ready to play the butler game
void readyToPlayButler(){
  
  Serial.println("Ready to play the game...");
  StaticJsonDocument<65> doc;

  doc["status"] = "true";
  doc["game"] = "butler";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}

//Tells the server that the robot can not start the butler game
void cantStartGameButler(){
  
  Serial.println("Unable to play the current game...");
  StaticJsonDocument<50> doc;

  doc["status"] = "false";
  doc["game"] = "butler";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}

//Handles the server request to the robot to start the game
void gameStartServerSideMaze() {
  StaticJsonDocument<65> doc;

  char json[] = "{\"action\":\"start\"game\":\"maze\"}";

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char action = doc["action"];
  const char game = doc["game"];
  Serial.println(action);
  Serial.println(game);
}

//Handles the server request to the robot to stop the butler game.
void gameEndedServerSideMaze() {
  StaticJsonDocument<65> doc;

  char json[] = "{\"action\":\"ended\"game\":\"maze\"}";

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char action = doc["action"];
  const char game = doc["game"];
  Serial.println(action);
  Serial.println(game);
}
//Handles the server request to the robot to start the butler game
void gameStartServerSideButler() {
  StaticJsonDocument<65> doc;

  char json[] = "{\"action\":\"start\"game\":\"butler\"}";

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char action = doc["action"];
  const char game = doc["game"];
  Serial.println(action);
  Serial.println(game);
}

//Handles the server request to the robot to stop the butler game.
void gameEndedServerSideButler() {
  StaticJsonDocument<65> doc;

  char json[] = "{\"action\":\"ended\"game\":\"butler\"}";

  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char action = doc["action"];
  const char game = doc["game"];
  Serial.println(action);
  Serial.println(game);
}


//Sends the acceleration of the robot to the server
void robotAcceleration(){
  
  Serial.println("Acceleration data: ");
  StaticJsonDocument<100> doc;

  doc["status"] = "in_game";
  doc["isDriving"] = "";
  doc["acceleration"] = "";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}

void sendData(){
  Serial.println("robotAcceleration");
  
}

//ERRORS

//Game not found
void errorGameNotFound(){
  
  Serial.println("Error: trying to find the specified game.");
  StaticJsonDocument<50> doc;

  doc["error"] = "GAME_NOT_FOUND";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}

//Game could not be prepared
void errorGameCouldNotPrepare(){
  
  Serial.println("Error: it was not possible to prepare the game.");
  StaticJsonDocument<50> doc;

  doc["error"] = "GAME_NOT_PREPARED";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}

//Unable to finish
void errorRobotCantFinish(){
  
  Serial.println("Error: Robot could not finish.");
  StaticJsonDocument<50> doc;

  doc["error"] = "UNABLE_TO_FINISH";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}

//Already playing the game
void gameAlreadyPlaying(){
  
  Serial.println("Error: Game is already being played");
  StaticJsonDocument<50> doc;

  doc["error"] = "ALREADY_PLAYING_GAME";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}

//Invalid Json file
void invalidJsonFile(){
  
  Serial.println("Error: invalid JSON file");
  StaticJsonDocument<50> doc;

  doc["error"] = "INVALID_JSON";

  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, Serial);
  Serial.println();
}

void loop() {
    webSocket.loop();
//    websocket.sendTXT("Hallo");
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED: {
      Serial.println("[WS] Disconnected!");
      break;
    }
    case WStype_CONNECTED: {
      Serial.println("[WS] Connected to: " + String((char * )payload));

      // send message to server when Connected
      Serial.println("{\"action\": \"login\", \"id\": \"" + WiFi.macAddress() + "\"}");
      webSocket.sendTXT("{\"action\": \"login\", \"id\": \"" + WiFi.macAddress() + "\"}");
      break;
    }
    case WStype_TEXT: {
      Serial.println("[WS] server says: " + String((char * )payload));
      StaticJsonDocument<50> doc;
      deserializeJson(doc, payload);
      
      const char* error = doc["error"];
      
      if(error){
        //Oh god.
        Serial.println("Error");
        return;
      }

      const char* loggedin = doc["loggedin"];

      /*if(loggedin){
        //Login message
        Serial.println("Ik ben ingelogd :D");
        return;
      }*/

      const char* game = doc["game"];
      const char* action = doc["action"];

      Serial.println(game);

      // send message to server
        //webSocket.sendTXT("Testing");
      break;
    }
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}
