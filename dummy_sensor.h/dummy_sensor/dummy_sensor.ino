#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include "secrets.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

//Parameters 

long randDifference;
const char* PlantGroup = "Living Room Corner";    

// Sensor 1 Struct //

struct sensor
{
  long Value;
  const char* Plant; 
  const char* Type; 
  const int ID; 
};

struct sensor sensor1{50,"Fancy Ficus","Soil Moisture",1};
// Sensor 2 

// Sensor 2 end

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
    


    // Serial.println(SensorValue);
    Serial.print(sensor1.Plant);
      while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      
    send(PlantGroup,sensor1.Plant,sensor1.Type,sensor1.ID,sensor1.Value);
    sensor1.Value = dummy_value(sensor1.Value) ;

    // Add Additional Sensor Readings Here
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
void send(const char* PlantGroup,const char* Plant,const char* Sensor,int Sensor_ID,int Sensor_Value)
{
    StaticJsonDocument<200> doc;
    doc["PlantGroup"] = PlantGroup;
    doc["Plant"] = Plant;
    doc["Sensor"] = Sensor; 
    doc["Sensor_ID"] = Sensor_ID; 
    doc["SensorValue"] = Sensor_Value;
    char buffer[256];
    serializeJson(doc, buffer);
    Serial.print(buffer);
    client.publish("Plants", buffer);
}

int dummy_value(int currentValue){
    randDifference = random(-5, 11);
    currentValue = currentValue + randDifference;
    if (currentValue <= 0) {
      currentValue = 0;
    }
    else if (currentValue >= 100) {
      currentValue = 100;
      }
    else {
    }
    return currentValue;
}
