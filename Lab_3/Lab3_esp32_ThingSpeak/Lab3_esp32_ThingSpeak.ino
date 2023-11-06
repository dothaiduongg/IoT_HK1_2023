#include <WiFi.h>
#include "ThingSpeak.h"
#include <DHT.h>

#define WIFI_SSID "Duongc3kt"
#define WIFI_PASSWORD "ouduorone110403"

#define SECRET_CH_ID 2332495
#define SECRET_WRITE_APIKEY "4RM0O1IIQFLCTIUI" 


#define DHTPIN 14 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

unsigned long dataMillis = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
  Serial.print(".");
  delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  ThingSpeak.begin(client);
}

void loop() {

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;

    }

    Serial.print("Nhiet do: ");
    Serial.print(t);
    Serial.print("*C ");
    Serial.print("Do am: ");
    Serial.print(h);
    Serial.println("% ");

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  if (millis() - dataMillis > 20000)
  {
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
   if(x == 500){
   Serial .println("Channel update successful.");
  } 
    else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
   } 
  }
}