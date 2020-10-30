#include "secrets.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Ethernet.h>

//Parameters 

const char* PlantGroup = "Living_Room_";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 51);
IPAddress myDns(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 0, 0);
// Sensor 1 Struct //

struct sensor
{
  const bool avail;
  float value;
  const int index;
};

struct sensor sensor0{true ,NULL,0};
struct sensor sensor1{true ,NULL,1};
struct sensor sensor2{true ,NULL,2};
struct sensor sensor3{true ,NULL,3};
struct sensor sensor4{false ,NULL,4};


unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
EthernetClient ethClient;
PubSubClient client(ethClient);
  
void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  int i = 0;
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  client.setServer(MQTTBROKER, 1883);

  }


void loop() {
    while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
    // initalize JSON // 
    StaticJsonDocument<200> doc;
    doc["Plant_Group"] = PlantGroup;
    JsonArray sensor = doc.createNestedArray("sensor");
    // print out the value you read:
      if (sensor0.avail == true){
      sensor0.value = analogRead(A0);
      JsonObject sensor_0 = sensor.createNestedObject();
      sensor_0["index"] = sensor0.index;
      sensor_0["value"] = sensor0.value;
      };
      if (sensor1.avail == true){
      sensor1.value = analogRead(A1);
      JsonObject sensor_1 = sensor.createNestedObject();
      sensor_1["index"] = sensor1.index;
      sensor_1["value"] = sensor1.value;
      };
      if (sensor2.avail == true){
      sensor2.value = analogRead(A2);
      JsonObject sensor_2 = sensor.createNestedObject();
      sensor_2["index"] = sensor2.index;
      sensor_2["value"] = sensor2.value;
      };
      if (sensor3.avail == true){
      sensor3.value = analogRead(A3);
      JsonObject sensor_3 = sensor.createNestedObject();
      sensor_3["index"] = sensor3.index;
      sensor_3["value"] = sensor3.value;
      };





    // Add Additional Sensor Readings Here
    char buffer[256];
    serializeJson(doc, buffer);
    Serial.println(buffer);
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

 float sensorread (int reading, const float high, const float low) {
      return (1 - ((reading - high) / (low - high)));
    };
