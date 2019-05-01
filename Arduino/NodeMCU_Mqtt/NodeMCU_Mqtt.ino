#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

// constants for the specific network
const char* ssid = "modellfabrik";
const char* password = "modellfabrikHTWG_I4.0";
const char* mqtt_server = "141.37.169.17";

// client for the commication with mqtt
WiFiClient espClient;
PubSubClient client(espClient);

SoftwareSerial interSerial(D6, D5);         // D6 = RX, D5 = TX

void setup() {  
  
  Serial.begin(74880);

  delay(100);
  
  Serial.println("Start SetUp");

  delay(100);
  
  interSerial.begin(74880);
  Serial.println("SetUp Serial communication");
  
  delay(100);

  setup_wifi();
  Serial.println("SetUp WiFi");

  delay(1000);

  // SetUp the client with the mqtt_server and the callback function from mqtt
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttCallback);
  Serial.println("SetUp MQTT");

  delay(1000);
 

}

void setup_wifi() {
   delay(100);
   
    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
    
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("patho/moco/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} //end reconnect()

void mqttCallback(char* topic, byte* payload, unsigned int length) 
{
  String message = "";

  for(int i = 0; i < length; i++) {
    message = message + (char) payload[i];
  }

  //Serial.println("Command from MQTT broker is : [" + String(topic) + "] Message: " + message);  
   
  int value = message.toInt();

  String messageToSend = "";
      
  if (strcmp(topic, "patho/moco/Servo_0") == 0) {
    messageToSend = "0|" + (String)value + "#";
  } else if (strcmp(topic, "patho/moco/Servo_1") == 0) {
    messageToSend = "1|" + (String)value + "#";
  } else if (strcmp(topic, "patho/moco/Servo_2") == 0) {
    messageToSend = "2|" + (String)value + "#";
  } else if(strcmp(topic, "patho/moco/Servo_3") == 0){
    messageToSend = "3|" + (String)value + "#";
  } else if (strcmp(topic, "patho/moco/Servo_4") == 0) {
    messageToSend = "4|" + (String)value + "#";
  } else    if (strcmp(topic, "patho/moco/Servo_5") == 0) {
    messageToSend = "5|" + (String)value + "#";
  } 
    
  interSerial.print(messageToSend);
  
  //Serial.println("Send to mega: " + messageToSend);
}
