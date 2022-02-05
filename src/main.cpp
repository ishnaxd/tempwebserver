
#include "ESP8266WiFi.h"

#include <ESPAsyncWebServer.h>
#include "SPIFFSEditor.h"
#include "Arduino.h"
#include <Arduino_JSON.h>
#include "dht.h"
#include <DHT_U.h>

const char* ssid = "FTTH-BSNL";
const char* password = "home@911";
#define DHTPIN 4
#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

AsyncWebServer server(80);
String readDHTTemperature() {

  float t = dht.readTemperature();


  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}
String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
dht.begin();
// Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
// Connect to Wi-Fi
// WiFi.mode (WIFI_STA);

  // WiFi_set_ps(WIFI_PS_NONE);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
// Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
// Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest   *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
// Start server
  server.begin();
}
 
void loop(){
  
}