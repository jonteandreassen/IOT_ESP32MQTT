
#include "DHT.h"
#include "PubSubClient.h" 
#include "WiFi.h" 
#define DHTPIN 32  
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE); // initialize DHT sensor

// WiFi Settings
const char* ssid = "JontesWiFi";                 
const char* wifi_password = "%casaDeJonte"; 

// MQTT
const char* mqtt_server = "192.168.0.117";  // IP of the MQTT broker
const char* humidity_topic = "home/humidity";
const char* temperature_topic = "home/temperature";
const char* mqtt_username = "test"; // MQTT username
const char* mqtt_password = "test"; // MQTT password
const char* clientID = "ESP32"; // MQTT client ID


WiFiClient wifiClient; // Initialise the WiFi and MQTT Client objects
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker


// Function to connet to the MQTT broker via WiFi
void connect_MQTT(){
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, wifi_password);  // Connect to the WiFi

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT Broker
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  connect_MQTT();
}

void loop() {


  Serial.setTimeout(2000);
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  
  if(isnan(temp) || isinf(hum)) {
    Serial.println("Failed to read DATA from DHT");
    delay(1000); // purpose of delay is that hopefully after 1 sec it starts reading again
  } else {
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println(" %");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
  }

  // Convert float Temp and hum to string, MQTT only supports String
  String humString="Hum: "+String((float)hum)+" %";
  String tempString="Temp: "+String((float)temp)+" C";

  // TEMPERATURE
  if (client.publish(temperature_topic, tempString.c_str())) {
    Serial.println("Temperature sent!");
  }
  else {
    Serial.println("Temperature failed to send.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(temperature_topic, tempString.c_str());
  }

  // HUMIDITY 
  if (client.publish(humidity_topic, humString.c_str())) {
    Serial.println("Humidity sent!");
  }
  else {
    Serial.println("Humidity failed to send.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); 
    client.publish(humidity_topic, humString.c_str());
  }
  
  //client.disconnect();  // disconnect from the MQTT broker
  delay(1000*10);
  client.loop(); 
}