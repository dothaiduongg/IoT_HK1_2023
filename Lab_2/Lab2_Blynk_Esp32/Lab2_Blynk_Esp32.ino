#include <DHT.h>
#include <DHT_U.h>

#define BLYNK_TEMPLATE_ID "TMPL6eQRG5ql3" #define BLYNK_TEMPLATE_NAME "duchung"   
#define BLYNK_AUTH_TOKEN "LmmoDrv6UIAeLvcTXuzHyYSrpOeE9HBa" 

#define BLYNK_PRINT Serial
#define DHTTYPE DHT11
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);

#include<Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int LED_GREEN = 15;
int LED_RED = 23;
int trigger_pin = 5;
int echo_pin = 18;
int distance_cm;

char ssid[] = "Hungga";
char pass[] = "bo031203";

BlynkTimer timer;
void setup()
{
  Serial.begin(9600);
  timer.setInterval(200L, hcsr04);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}
void hcsr04()
{
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  long duration = pulseIn(echo_pin, HIGH);
  distance_cm = (duration / 2) / 29.09;
  Serial.println(distance_cm);
  if (isnan(distance_cm)) {
      Serial.println("Failed to read from HCSR04 sensor!");
      return;
  }
  Blynk.virtualWrite(V2, distance_cm);
  delay(200);
}
void sendSensor()
{
//float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if ( isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
  }
  Blynk.virtualWrite(V3, t);
  //Blynk.virtualWrite(V6, h);
}
void loop()
{
  hcsr04();
  Blynk.run();
  sendSensor();
  if(distance_cm <= 10 || distance_cm >= 300)
  {
    digitalWrite(LED_RED, HIGH);
    Blynk.virtualWrite(V1,1);
    /*delay(500);
    digitalWrite(LED_3,LOW);
    delay(500);*/
  }
  else { 
    digitalWrite(LED_RED, LOW);
    Blynk.virtualWrite(V1,0);
  }
}
BLYNK_WRITE(V0){
    int value = param.asInt();
    if (value == 1)
    digitalWrite(LED_GREEN, HIGH);
    else
    digitalWrite(LED_GREEN, LOW);
}
/*BLYNK_WRITE(V1){
int value = param.asInt();
if (value == 1)
digitalWrite(LED_RED, HIGH);
else
digitalWrite(LED_RED, LOW);
}*/
