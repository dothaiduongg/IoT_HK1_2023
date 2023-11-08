
//khai bao thu vien wifi va cam bien
#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define WIFI_SSID "Xuan Sang"           //ten Wifi hien tai
#define WIFI_PASSWORD "68686868"        //password

#define SECRET_CH_ID 2334577                          //Channel ID
#define SECRET_WRITE_APIKEY "S2YOVFS3QCHLEWC9"        //Write API key


#define DHTPIN 14                       //Gan chan cho esp
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;             //gan bien cho Channel ID
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;         //gan bien cho Write API key

unsigned long dataMillis = 0;                   //dieu khien delay

void setup() {
  Serial.begin(9600);                          //khai bao toc do serial
  delay(1000);


  //kiem tra ket noi wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
  Serial.print(".");
  delay(500);
  }


  //
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //khoi tao thingspeak
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

      //ket qua hien thi tren man hinh ung voi thu tu bieu do tren thingspeak
  ThingSpeak.setField(1, t);                       
  ThingSpeak.setField(2, h);

                                     
  if (millis() - dataMillis > 20000)        
  {
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);     //ghi du lieu dc set len giao dien channel
   if(x == 200){              
    //ham so sanh x == 200 vi trong phan write data cua mathwork cho giao thuc http, trang thai gui tin hieu thanh cong la 200, neu gui khac gia tri 200 thi tren thingspeak khong nhan gia tri
    //Link: https://ch.mathworks.com/help/thingspeak/writedata.html 
   Serial .println("Channel update successful.");
  } 
    else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
   } 
  }
}