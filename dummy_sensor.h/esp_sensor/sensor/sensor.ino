#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include "secrets.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Edit Parameters 
// For Plant Sensors add the Plant name 
// For Group Sensors add NULL as name


//Parameters 

long randDifference;
const char* PlantGroup = "Living Room Corner";    

// Sensor 1 Struct //

struct sensor
{
  long Value;
  const char* Plant; 
  const char* Type; 
  const int Hardware_ID; 
  const bool avail;
};

const int mplex1 = 5 ;
const int mplex2 = 4 ;
const int mplex3 = 12 ;
const int mplex4 = 13 ;

struct sensor sensor1{50,"Fancy Ficus","Soil Moisture",1,true};
struct sensor sensor2{50,"Magnificient Monstera","Soil Moisture",2,true};
struct sensor sensor3{50,"Amazing Aloe","Soil Moisture",3,true};
struct sensor sensor4{50,"Dramatic Dragon Tree","Soil Moisture",5,false};
struct sensor sensor5{50,NULL,"Brightness",5,false};
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


}


void loop() {
    


    // Serial.println(SensorValue);
    //Serial.print(sensor1.Plant);
      while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
  // print out the value you read:
    if (sensor1.avail == true) {
    multiplex(LOW,LOW,LOW,LOW);
    delay(50);
    sensor1.Value = analogRead(A0);
    send(PlantGroup,sensor1.Plant,sensor1.Type,sensor1.Hardware_ID,sensor1.Value);
    delay(50);
    }
    if (sensor2.avail == true) {
    multiplex(HIGH,LOW,LOW,LOW);
    delay(50);
    sensor2.Value = analogRead(A0);
    send(PlantGroup,sensor2.Plant,sensor2.Type,sensor2.Hardware_ID,sensor2.Value);
    delay(50);
    }
    if (sensor3.avail == true) {
      multiplex(LOW,HIGH,LOW,LOW);
      delay(50);
      sensor3.Value = analogRead(A0);
      send(PlantGroup,sensor3.Plant,sensor3.Type,sensor3.Hardware_ID,sensor3.Value);
    }
    if (sensor4.avail == true) {
    digitalWrite(0,LOW);
    digitalWrite(1,LOW);
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    sensor4.Value = analogRead(A0);
    send(PlantGroup,sensor4.Plant,sensor4.Type,sensor4.Hardware_ID,sensor4.Value);
    }
    if (sensor4.avail == true) {
    send(PlantGroup,sensor5.Plant,sensor5.Type,sensor5.Hardware_ID,sensor5.Value);
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


void send(const char* PlantGroup,const char* Plant,const char* Sensor,int Sensor_ID,int Sensor_Value)
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
}
void multiplex(int pin1,int pin2,int pin3,int pin4){
    digitalWrite(mplex1,pin1);
    digitalWrite(mplex2,pin2);
    digitalWrite(mplex3,pin3);
    digitalWrite(mplex4,pin4);
}
