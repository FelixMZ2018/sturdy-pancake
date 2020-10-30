#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include "secrets.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Edit Parameters 
// For Plant Sensors add the Plant name 
// For Group Sensors add NULL as name

//Parameters 

const char* PlantGroup = "Living_Room_";

// Sensor 1 Struct //

struct sensor
{
  const bool avail;
  float value;
  const int index;
};
const int mplex1 = 5 ;
const int mplex2 = 4 ;
const int mplex3 = 12 ;
const int mplex4 = 13 ;


struct sensor sensor0{true ,NULL,0};
struct sensor sensor1{true ,NULL,1};
struct sensor sensor2{true ,NULL,2};
struct sensor sensor3{true ,NULL,3};
struct sensor sensor4{false ,NULL,4};

//Plant Name Group equals group sensor //

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
  WiFi.mode(WIFI_STA);


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
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);
  digitalWrite(12,LOW);
  digitalWrite(13,LOW);  

}


void loop() {
    


    // Serial.println(Sensorvalue);
    //Serial.print(sensor1.Plant);
      while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
  // print out the value you read:
    StaticJsonDocument<200> doc;
    doc["Plant_Group"] = PlantGroup;
    JsonArray sensor = doc.createNestedArray("sensor");
    if (sensor0.avail == true) {
    multiplex(LOW,LOW,LOW,LOW);
    digitalWrite(5,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);  
    delay(50);
    sensor0.value = analogRead(A0);
    JsonObject sensor_0 = sensor.createNestedObject();
    sensor_0["index"] = sensor0.index;
    sensor_0["value"] = sensor0.value;
    delay(50);
    }
    if (sensor1.avail == true) {
    digitalWrite(5,HIGH);
    digitalWrite(4,LOW);
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);    
    delay(50);
    sensor1.value = analogRead(A0);
    JsonObject sensor_1 = sensor.createNestedObject();
    sensor_1["index"] = sensor1.index;
    sensor_1["value"] = sensor1.value;
    delay(50);
    }
    if (sensor2.avail == true) {
    digitalWrite(5,LOW);
    digitalWrite(4,HIGH);
    digitalWrite(12,LOW);
    digitalWrite(13,LOW); 
    delay(50);
    sensor2.value = analogRead(A0);
    JsonObject sensor_2 = sensor.createNestedObject();
    sensor_2["index"] = sensor2.index;
    sensor_2["value"] = sensor2.value;
    }
    if (sensor3.avail == true) {
    digitalWrite(5,LOW);
    digitalWrite(4,LOW);
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
    sensor3.value = analogRead(A0);
    JsonObject sensor_3 = sensor.createNestedObject();
    sensor_3["index"] = sensor3.index;
    sensor_3["value"] = sensor3.value;
    }
    if (sensor4.avail == true) {
    }
    //Serial.println(String(random(0xffffffff), HEX));
    char buffer[256];
    serializeJson(doc, buffer);
    Serial.println(buffer);
    client.publish("Plants", buffer);

    // Add Additional Sensor Readings Here
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(500);
    }
  }
}

void multiplex(int pin1,int pin2,int pin3,int pin4){
    digitalWrite(mplex1,pin1);
    digitalWrite(mplex2,pin2);
    digitalWrite(mplex3,pin3);
    digitalWrite(mplex4,pin4);
}
