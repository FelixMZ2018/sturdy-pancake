#include "secrets.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Ethernet.h>


// Edit Parameters 
// For Plant Sensors add the Plant name 
// For Group Sensors add NULL as name


//Parameters 

long randDifference;
const char* PlantGroup = "Living Room Corner";    
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 51);
IPAddress myDns(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 0, 0);
// Sensor 1 Struct //

struct sensor
{
  float Value;
  const char* Plant; 
  const char* Type; 
  const int Hardware_ID; 
  const bool avail;
};

struct sensor_template
{
  const char* name;
  const float high;
  const float low;
}; 

struct sensor_template soil_moisture{"Soil Moisture",400,750};

struct sensor sensor0{50,"Fancy Ficus","Soil Moisture",0,true};
struct sensor sensor1{50,"Magnificient Monstera","Soil Moisture",1,true};
struct sensor sensor2{50,"Amazing Aloe","Soil Moisture",2,true};
struct sensor sensor3{50,"Dramatic Dragon Tree","Soil Moisture",3,true};
struct sensor sensor4{50,"Beautiful Begonia","Soil Moisture",4,false};
struct sensor sensor5{50,NULL,"Brightness",5,true};
//Plant Name Group equals group sensor //

// Sensor 2 

// Sensor 2 end

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
  // print out the value you read:
    if (sensor0.avail == true) {
    Serial.print("Sensor 0 Reading:");
    Serial.print(analogRead(A0));
    sensor0.Value = (sensorread(analogRead(A0),soil_moisture.high,soil_moisture.low));
    send(PlantGroup,sensor0.Plant,sensor0.Type,sensor0.Hardware_ID,sensor0.Value);
    delay(50);
    }
    if (sensor1.avail == true) {
    Serial.print("Sensor 1 Reading:");
    Serial.print(analogRead(A1));
    sensor1.Value = (sensorread(analogRead(A1),soil_moisture.high,soil_moisture.low));
    send(PlantGroup,sensor1.Plant,sensor1.Type,sensor1.Hardware_ID,sensor1.Value);
    delay(50);
    }
    if (sensor2.avail == true) {
    Serial.print("Sensor 2 Reading:");
    Serial.print(analogRead(A2));
    sensor2.Value = (sensorread(analogRead(A2),soil_moisture.high,soil_moisture.low));
    send(PlantGroup,sensor2.Plant,sensor2.Type,sensor2.Hardware_ID,sensor2.Value);
    }
    if (sensor3.avail == true) {
    Serial.print("Sensor 3 Reading:");
    Serial.print(analogRead(A3));
    sensor3.Value = (sensorread(analogRead(A3),soil_moisture.high,soil_moisture.low));
    send(PlantGroup,sensor3.Plant,sensor3.Type,sensor3.Hardware_ID,sensor3.Value);
    }
    if (sensor4.avail == true) {
    Serial.print("Sensor 4 Reading:");
    Serial.print(analogRead(A4));
    sensor4.Value = (sensorread(analogRead(A4),soil_moisture.high,soil_moisture.low));
    send(PlantGroup,sensor4.Plant,sensor4.Type,sensor4.Hardware_ID,sensor4.Value);
    }
    //Serial.println(String(random(0xffffffff), HEX));


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


void send(const char* PlantGroup,const char* Plant,const char* Sensor,int Sensor_ID,float Sensor_Value)
{
    StaticJsonDocument<200> doc;
    doc["Plant_Group"] = PlantGroup;
    doc["Plant_Name"] = Plant;
    doc["Sensor_Type"] = Sensor; 
    doc["Sensor_ID"] = Sensor_ID; 
    doc["Sensor_Value"] = Sensor_Value;
    char buffer[256];
    serializeJson(doc, buffer);
    Serial.println(buffer);
    client.publish("Plants", buffer);
}
 float sensorread (int reading, const float high, const float low) {
      return (1 - ((reading - high) / (low - high)));
    }
    
