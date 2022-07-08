#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

//-----------MQ137 sensor amônia--------------------
#define RL 9.5  //The value of resistor RL is 47K

#define m -0.263 //Enter calculated Slope 

#define b 0.42 //Enter calculated intercept

#define Ro 14.5 //Enter found Ro value

#define MQ_sensor 32 //Sensor is connected to A4

//-------------------------------------------------

//----------------dht sensor temp e humidade-------
#include "DHT.h"

#define DHTPIN 17 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
//------------------------------------------------

const char* ssid ="Bruno Migliorini";
const char* password="edicula88";

char jsonOutput[256];
float ppm=0.0;
float humidade=0.0;
float temperatura=0.0;


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.print("Conectando ao wifi");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
  dht.begin();
  pinMode(MQ_sensor,INPUT);
}

void loop() 
{

  
  leituraAmonia();
  delay(100);
  leitura_sensor_dht22();
  
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
      object["amonia"] = ppm;
      object["humidade"]=humidade;
      

      serializeJson(doc, jsonOutput);

      int httpCode = client.POST(String(jsonOutput));
      if(httpCode > 0){
        String payload = client.getString();
        Serial.println("\nStatusCode: " + String(httpCode));
        Serial.println(payload);
        
      }
      else
      {
        Serial.println("sem resposta do servidor");
      }
  client.end();
  }
 
 delay(4000);

}

void leituraAmonia(){
  float VRL; //Voltage drop across the MQ sensor

  float Rs; //Sensor resistance at gas concentration 

  float ratio; //Define variable for ratio

   

  VRL = analogRead(MQ_sensor)*(3.3/4096.0); //Measure the voltage drop and convert to 0-3.1V

  Rs = ((4.7*RL)/VRL)-RL; //Use formula to get Rs value

  ratio = Rs/Ro;  // find ratio Rs/Ro

 

  ppm = pow(10, ((log10(ratio)-b)/m)); //use formula to calculate ppm


  Serial.print("NH3 (ppm) = "); //Display a ammonia in ppm

  Serial.println(ppm);


  Serial.print("Voltage = "); //Display a intro message 

  Serial.print(VRL);
}
void leitura_sensor_dht22(){
  humidade = dht.readHumidity();
  temperatura = dht.readTemperature();
  Serial.println(temperatura);
  Serial.println(humidade);
}
