#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "DHT.h"

#define DHTPIN 17 
#define DHTTYPE DHT22

const char* ssid ="Bruno Migliorini";
const char* password="edicula88";

char jsonOutput[128];

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.print("Conectando ao wifi");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
dht.begin();
}

void loop() 
{

  float humidade = dht.readHumidity();
  float temperatura = dht.readTemperature();
  Serial.println(temperatura);
  Serial.println(humidade);
  
  if(WiFi.status() == WL_CONNECTED)
  {
      Serial.println("conectado");
      HTTPClient client;
      client.begin("http://15.228.187.254:5000/sensores");
      
      client.addHeader("Content-Type", "application/json");

      const size_t CAPACITY = JSON_OBJECT_SIZE(4);
    
      StaticJsonDocument<CAPACITY> doc;

      JsonObject object = doc.to<JsonObject>();
      object["temperatura"] = temperatura;
      object["amonia"] = humidade;
      

      serializeJson(doc, jsonOutput);

      int httpCode = client.POST(String(jsonOutput));
      if(httpCode > 0){
        String payload = client.getString();
        Serial.println("\nStatusCode: " + String(httpCode));
        Serial.println(payload);
        client.end();
      }
      else
      {
        Serial.println("Connection Lost");
      }
  }
 delay(4000);

}
