//Websocket client
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
#include <sstream>
#include <string>
#include <iostream>

//Motor pins.
int rightBack = 16;
int rightFront = 17;
int leftBack = 18;
int leftFront = 5;
//Tracks driving and start status.
boolean isStarted = false;
boolean isDriving = false;
boolean mpuIsWorking = false;
//True if a critical error has occured and stops execution of program.
boolean stop = false;
//Used for tracking status and current game/task.
char *status = "preparing";
char *currentJob = "";
//Used for non-blocking delays.
unsigned long loopDelay = 0;
unsigned long statusDelay = 0;
//IR Sensor values, is used by all games.
const int RIGHT_IR_SENSOR = 39;
const int LEFT_IR_SENSOR = 34;
int rightIRValue = 0;
int leftIRValue = 0;
//Connecting to WiFi.
//const char *SSID = "battlebot_nl";
//const char *PASSWORD = "battlebot_nl";
const char *SSID = "iPhone";
const char *PASSWORD = "truppy123";
//Network data, Interface group.
//const char HOST[] = "10.110.111.103";
const char HOST[] = "172.20.10.6";
const char PATH[] = "/";
const int PORT = 3003;
//Screen resolution.
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 32;
WiFiClient client;
WebSocketsClient webSocket;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MPU9250 mpu;

boolean ready = false;

void setup()
{
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

    if(!mpu.setup(0x68))
    {
      Serial.println("Error starting MPU. ");
      //currentJob = "Error starting MPU.";
      //logScreen();
      //stop = true;
      //return;
    }
    else
    {
      mpuIsWorking = true;
      currentJob = "Calibrating in 5 seconds.";
      logScreen();
      delay(5000);
      currentJob = "Calibrating.";
      logScreen();
      mpu.calibrateAccelGyro();
    }
    //Start WiFi connection.
    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
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

    //server address, port and URL
    webSocket.begin(HOST, PORT, PATH);

    //Event handler
    webSocket.onEvent(webSocketEvent);

    //Tries to reconnect every 5 seconds.
    webSocket.setReconnectInterval(5000);

    status = "ready";
    currentJob = "Waiting for command.";
}


void loop()
{
    //If a critical error has occured, stop running the loop.
    if (stop)
    {
        Serial.println("Stopped");
        return;
    }
    //Send the status to the websocket server.
    sendStatus();
    //If there is a delay active.
    if (millis() < loopDelay)
    {
        Serial.println("Delay");
        return;
    }
    webSocket.loop();
    //Run game loop code if a game is running.
    if (currentJob == "race")
    {
        handleRaceGame();
    } else if (currentJob == "butler")
    {
        handleButlerGame();
    }
    logScreen();
    //Test code.
    //prepareRace();
}

boolean handleGameLoop(){
      if (stop)
    {
      drive(0, 0, 0, 0);
        Serial.println("Stopped");
        return true;
    }
    sendStatus();
    webSocket.loop();
    logScreen();
    return status == "finish";
}

void logScreen()
{
    //preparing       - De robot is nog niet klaar met opstarten
    //preparing_game  - De robot is bezig met het voorbereiden van een spel
    //ready           - De robot is klaar om een verzoek te ontvangen/spel starten/spel voorbereiden
    //in_game         - De robot is bezig met een spel
    //finished        - De robot is klaar met het spel en bij het einde aangekomen
    //Serial.println(currentJob);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Status: ");
    display.print(status);
    display.println("");
    display.print(currentJob);
    display.println("");
    display.print("INF1C Robot");
    display.display();
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    Serial.println(type);
    switch (type)
    {
        case WStype_DISCONNECTED:
        {
            currentJob = "[WS] Disconnected!";
            stop = true;
            return;
        }
        case WStype_CONNECTED:
        {
            Serial.println("[WS] Connected to: " + String((char *) payload));

            // send message to server when Connected
            webSocket.sendTXT("{\"action\": \"login\", \"id\": \"" + WiFi.macAddress() + "\"}");
            break;
        }
        case WStype_TEXT:
        {
            Serial.println("[WS] server says: " + String((char *) payload));
            StaticJsonDocument<50> doc;
            deserializeJson(doc, payload);

            const char *error = doc["error"];

            if (error)
            {
                //Oh god.
                Serial.println("Error");
                return;
            }

            bool loginResponse = doc.containsKey("loggedin");
            if (loginResponse)
            {
                Serial.println("Hallo!!");
                JsonVariant loginStatus = doc["loggedin"];
                bool loginStatusBool = loginStatus.as<bool>();
                if (loginStatusBool)
                {
                    currentJob = "Logged in.";
                    logScreen();
                } else
                {
                    currentJob = "Error logging in.";
                    stop = true;
                }
            }

            JsonVariant game = doc["game"];
            JsonVariant action = doc["action"];

            String sGame = game.as<String>();
            String sAction = action.as<String>();

            Serial.println("Ja!!");

            /*if(sGame == NULL){
             return;
            }*/


            Serial.println(sGame);

            Serial.println(sAction);

            gameCommand(sGame, sAction);
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

void gameCommand(String game, String action)
{
    //Prepare, start, stop.
    if (action == "prepare")
    {
        if (status == "in_game")
        {
            webSocket.sendTXT("{\"error\": \"ALREADY_PLAYING_GAME\"}");
            return;
        }
        Serial.println("PREPARE!!!");
        status = "preparing_game";
    } else if (action == "start")
    {
        if (status == "in_game")
        {
            webSocket.sendTXT("{\"error\": \"ALREADY_PLAYING_GAME\"}");
            return;
        }
        if (status != "preparing_game")
        {
            webSocket.sendTXT("{\"error\": \"GAME_NOT_PREPARED\"}");
            return;
        }

        Serial.println("IN_GAME!!!");
        status = "in_game";
    } else if (action == "ended")
    {
        status = "finished";
    }

    if (game == "race")
    {
        handleRaceGame();
    } else if (game == "butler")
    {
        handleButlerGame();
    } else if (game == "maze")
    {

    } else
    {
        webSocket.sendTXT("{\"error\": \"GAME_NOT_FOUND\"}");
    }
}

void sendStatus()
{
  float accel = 0;
  if(mpuIsWorking)
  {
    if(mpu.update())
    {
      accel = mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY;
      Serial.print(accel);
      }
     }
    if (millis() < statusDelay)
    {
        //Serial.println("Delay2");
        return;
    }

    //const char* cStatus = status;
    std::ostringstream ss;
    ss << accel;

    std::string str = "{\"status\": \"" + std::string(status) + "\", \"isDriving\": " +
                      (isDriving ? "true" : "false") + ", \"acceleration\": " + ss.str() + "}";
    Serial.println(str.c_str());
    webSocket.sendTXT(str.c_str());
    statusDelay = millis() + 5000;

}
