#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include "secrets.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>


long randDifference;
long sensorValue1 = 50;
const char* PlantGroup = "Living Room Corner";    
const char* Plant1 = "Fancy Ficus"; 
const char* Sensor1 = "Moisture" ; 

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(SSID, PASSWORD);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(SSID); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  client.setServer(MQTTBROKER, 1883);
}


void loop() {
    


    // Serial.println(sensorValue1);
    
      while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      
    randDifference = random(-5, 11);
    sensorValue1 = sensorValue1 + randDifference;
    if (sensorValue1 <= 0) {
      sensorValue1 = 0;
    }
    else if (sensorValue1 >= 100) {
      sensorValue1 = 100;
      }
    else {
    }

    StaticJsonDocument<200> doc;
    doc["PlantGroup"] = PlantGroup;
    doc["Plant"] = Plant1;
    doc["Sensor"] = Sensor1; 
    doc["SensorValue"] = sensorValue1;
    char buffer[256];
    serializeJson(doc, buffer);
    Serial.print(buffer);
    client.publish("Plants", buffer);
    delay(1000);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}
