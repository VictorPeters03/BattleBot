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
