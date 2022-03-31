//Websocket client
//Datum: 22 maart

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_VL53L0X.h>
#include <analogWrite.h>
#include <Wire.h>
#include <MPU9250.h>

int rightBack = 16;
int rightFront = 17;
int leftBack = 18; 
int leftFront = 5;
boolean isStarted = false;
boolean isDriving = false;
boolean stop = false;
char* status = "preparing";
char* currentJob = "";
unsigned long loopDelay = 0;

const int RIGHT_IR_SENSOR = 39;
const int LEFT_IR_SENSOR = 34;
int rightIRValue = 0;
int leftIRValue = 0;

//Connecting to WiFi.
const char* SSID = "battlebot_nl";
const char* PASSWORD = "battlebot_nl";
//Network data, Interface group.
const char HOST[] = "10.110.111.103";
const char PATH[] = "/";
const int PORT = 3003;
const int SCREEN_WIDTH = 128; // OLED display width, in pixels
const int SCREEN_HEIGHT = 32; // OLED display height, in pixels

WiFiClient client;
WebSocketsClient webSocket;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MPU9250 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
      //Setup the motors
  pinMode(rightFront, OUTPUT);
  pinMode(rightBack, OUTPUT);
  pinMode(leftFront, OUTPUT);
  pinMode(leftBack, OUTPUT);
  //Set all motors to 0 so they stop.
  analogWrite(leftFront, 0);
  analogWrite(rightFront, 0);
  analogWrite(leftBack, 0);
  analogWrite(rightBack, 0);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  delay(2000);

  /*if(!mpu.setup(0x68)){
    currentJob = "Error starting MPU.";
    logScreen();
    stop = true;
    return;
  }
  currentJob = "Calibrating in 5 seconds.";
  logScreen();
  delay(5000);
  currentJob = "Calibrating.";
  logScreen();
  mpu.calibrateAccelGyro();*/

  //Start WiFi connection.
  WiFi.begin(SSID, PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    currentJob = "Pending connection...";
    logScreen();
    delay(500);
  } 
  currentJob = "Connected.";
  logScreen();
  Serial.println("The IP-Adress is: ");
  Serial.println(WiFi.localIP().toString());
  Serial.println("The MAC-Address is: ");
  Serial.println(WiFi.macAddress());
  
  // server address, port and URL
  webSocket.begin(HOST, PORT, PATH);

  //Event handler
  webSocket.onEvent(webSocketEvent);
  
  //Tries to reconnect every 5 seconds. 
  webSocket.setReconnectInterval(5000);

  status = "ready";
}


void loop() {
  if(stop){
    return;
  }
  sendStatus();
  if(millis() < loopDelay){
    return;
  }
  if(currentJob == "race"){
    raceGame();
  } else if(currentJob == "butler"){
    butlerGame();
  } else {
    currentJob = "Waiting for command.";
    logScreen();
  }
  webSocket.loop();
  prepareRace();
}

void logScreen(){
  //preparing       - De robot is nog niet klaar met opstarten
  //preparing_game  - De robot is bezig met het voorbereiden van een spel
  //ready           - De robot is klaar om een verzoek te ontvangen/spel starten/spel voorbereiden
  //in_game         - De robot is bezig met een spel
  //finished        - De robot is klaar met het spel en bij het einde aangekomen
  Serial.println(currentJob);
  display.clearDisplay();
  display.setCursor(0,0);
  display.print ("Status: ");
  display.print(status);
  display.println("");
  display.print(currentJob);
  display.println("");
  display.print("INF1C Robot");
  display.display();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED: {
      currentJob = "[WS] Disconnected!";
      stop = true;
      return;
    }
    case WStype_CONNECTED: {
      Serial.println("[WS] Connected to: " + String((char * )payload));

      // send message to server when Connected
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

      bool loginResponse = doc.containsKey("loggedin");
      if(loginResponse){
        JsonVariant loginStatus = doc["loggedin"];
        bool loginStatusBool = loginStatus.as<bool>();
          if(loginStatusBool){
            currentJob = "Logged in.";
            } else {
              currentJob = "Error logging in.";
              stop = true;
        }
      }

      const char* game = doc["game"];
      const char* action = doc["action"];

      Serial.println(game);

      Serial.println(action);
      gameCommand(game, action);
      break;
    }
    case WStype_ERROR:
      currentJob = "[WS] Error!";
      stop = true;
      return;      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}

void gameCommand(const char* game, const char* action){
  //Prepare, start, stop.

  if(game == "race"){
    handleRaceGame(action);
  } else if(game == "butler"){
    //
  } else {
    //GAME_NOT_FOUND
  }
  
}

void sendStatus(){
  //if(mpu.update()){
  //  Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
 // }
 
   //const char* cStatus = status;
   //const char* json = "{\"status\": \"" + cStatus + "\", \"isDriving\": " + isDriving + ", \"acceleration\": " + 0 + "}";
 
   //webSocket.sendTXT("{\"status\": \"" + cStatus + "\", \"isDriving\": " + isDriving + ", \"acceleration\": " + 0 + "}");
}
