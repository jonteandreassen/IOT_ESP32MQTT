
//#include "ESP32HTTPUpdateServer.h" //behövs den?
//#include "EspMQTTClient.h" // behövs den?



#include "DHT.h"
#include "PubSubClient.h" 

#include "WiFi.h" 
#define DHTPIN 32  


#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

// WiFi
const char* ssid = "JontesWiFi";                 
const char* wifi_password = "%casaDeJonte"; 

// MQTT
const char* mqtt_server = "192.168.0.117";  // IP of the MQTT broker
const char* humidity_topic = "home/livingroom/humidity";
const char* temperature_topic = "home/livingroom/temperature";
const char* mqtt_username = "test"; // MQTT username
const char* mqtt_password = "test"; // MQTT password
const char* clientID = "client_livingroom"; // MQTT client ID


WiFiClient wifiClient; // Initialise the WiFi and MQTT Client objects
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker


// Function to connet to the MQTT broker via WiFi
void connect_MQTT(){
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging - Output the IP Address of the ESP32
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
  
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  // MQTT can only transmit strings
  String hs="Hum: "+String((float)hum)+" %";
  String ts="Temp: "+String((float)temp)+" C";

  // TEMPERATURE
  if (client.publish(temperature_topic, String(temp).c_str())) {
    Serial.println("Temperature sent!");
  }
  else {
    Serial.println("Temperature failed to send.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(temperature_topic, String(temp).c_str());
  }

  // HUMIDITY 
  if (client.publish(humidity_topic, String(hum).c_str())) {
    Serial.println("Humidity sent!");
  }
  else {
    Serial.println("Humidity failed to send.");
    client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); 
    client.publish(humidity_topic, String(hum).c_str());
  }
  
  //client.disconnect();  // disconnect from the MQTT broker
  delay(1000*10);
  client.loop(); 
}