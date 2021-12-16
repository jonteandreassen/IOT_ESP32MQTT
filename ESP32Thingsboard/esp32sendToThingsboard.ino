#include "ThingsBoard.h"
#include "DHT.h"
#include "WiFi.h"

#define DHTPIN 32  
#define DHTTYPE DHT11
#define WIFI_AP             " ##### "  // SSID
#define WIFI_PASSWORD       " ##### "  
#define TOKEN               " ##### " // Device token generated at thingsboard
#define THINGSBOARD_SERVER  "thingsboard.cloud"
#define SERIAL_DEBUG_BAUD   115200 // Baud rate for debug serial

DHT dht(DHTPIN, DHTTYPE); // initialize DHT sensor
WiFiClient espClient; // Initialize ThingsBoard client
ThingsBoard tb(espClient); // Initialize ThingsBoard instance
int status = WL_IDLE_STATUS; // the Wifi radio's status


void InitWiFi(){
  Serial.println("Connecting to Wifi ...");
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to: ");
    Serial.println(WiFi.localIP());
  }
}


void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  dht.begin();
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  // variables to hold DHT sensor data
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  //if there is no data  
  if(isnan(temp) || isinf(hum)) {
    Serial.println("Failed to read DATA from DHT");
    delay(1000); // purpose of delay is that hopefully after 1 sec it starts reading again
  // if there is data continue and do this : 
  } else {
    Serial.println("Sending data to ThingsBoard: ");
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println(" %");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
  }
  // Send data to Thingsboard as float
  tb.sendTelemetryFloat("temperature", temp);
  tb.sendTelemetryFloat("humidity", hum);

  tb.loop();
  delay(1000 * 60); // 1 min delay
}


