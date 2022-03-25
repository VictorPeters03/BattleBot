//Websocket client
//Datum: 22 maart


#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

//Connecting to WiFi.
const char* ssid = "test";
const char* password = "95151779";

//Network data, Interface group.
const char host[] = "192.168.0.101";
const int port = 3003;
const char path[] = "";

WiFiClient client;
WebSocketsClient webSocket;

//Checked the SSID and Password. 
void setup() {
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

  //Server address, Port and the URL
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
    case WStype_DISCONNECTED:
      Serial.println("[WS] Disconnected!");
      break;
      
    case WStype_CONNECTED:
      Serial.println("[WS] Connected to: " + String((char * )payload));

      // send message to server when Connected
      webSocket.sendTXT("Connected: " + WiFi.macAddress());
      break;
      
    case WStype_TEXT:
      Serial.println("[WS] server says: " + String((char * )payload));

      // send message to server
        webSocket.sendTXT("Testing");
      break;
      
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}
