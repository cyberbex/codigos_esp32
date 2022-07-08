#include <WiFi.h>
#include <HTTPClient.h>


const char* ssid ="Bruno Migliorini";
const char* password="edicula88";


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.print("Conectando ao wifi");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }

}

void loop() {
  if(WiFi.status() == WL_CONNECTED)
  {
      Serial.println("conectado");
      HTTPClient client;
      client.begin("http://127.0.0.1:5000");
      client.addHeader("Content-Type", "text/plain");

      int httpCode = client.POST("88");
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
    delay(3000);

 }
 delay(3000);

}
