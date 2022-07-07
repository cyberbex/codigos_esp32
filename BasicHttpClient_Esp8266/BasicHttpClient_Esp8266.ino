#include <ArduinoJson.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>


#include <ESP8266HTTPClient.h>


char jsonOutput[128];

const char* ssid = "Bruno Migliorini"; //Enter SSID
const char* password = "edicula88"; //Enter Password

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print("*");
  }
  
  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.print(WiFi.localIP());// Print the IP address
}

void loop() {
  // wait for WiFi connection
  if(WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    WiFiClient client;

    Serial.print("[HTTP] begin...\n");
    http.begin(client,"http://15.228.187.254:5000/sensores");
     
        http.addHeader("Content-Type", "application/json");

        const size_t CAPACITY = JSON_OBJECT_SIZE(4);
    
        StaticJsonDocument<CAPACITY> doc;

        JsonObject object = doc.to<JsonObject>();
        object["temperatura"] = 8;
        object["amonia"] = 15;
      
        serializeJson(doc, jsonOutput);
        int httpCode = http.POST(String(jsonOutput));
      // httpCode will be negative on error
      if (httpCode > 0) 
      {
        String payload = http.getString();
        Serial.println("\nStatusCode: " + String(httpCode));
        Serial.println(payload);
        
      } else 
      {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
     
  }

  delay(10000);
}
