
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <DHT_U.h>

// Provide the RTDB payload printing info and other helper functions.
//#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Duongc3kt"
#define WIFI_PASSWORD "ouduorone110403" 

/* 2. Define the RTDB URL */
#define DATABASE_URL "mq2-dht11-ca281-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;


#define DHTPIN 4    // DHT11
#define DHTTYPE DHT11   // DHT 11
#define MQ2_A 35 // define MQ2 analog pin
#define MQ2_D 34 // define MQ2 digital pin
#define LEDXANH 13
#define LEDDO 14
#define LEDVANG 12 


int A_value, D_value;
DHT dht(DHTPIN, DHTTYPE);

unsigned long dataMillis = 0;

void setup()
{

    Serial.begin(9600);

    pinMode(MQ2_A, INPUT);
    pinMode(MQ2_D, INPUT);

    pinMode(LEDXANH, OUTPUT);
    digitalWrite(LEDXANH, LOW);

    pinMode(LEDVANG, OUTPUT);
    digitalWrite(LEDVANG, LOW);
    
    pinMode(LEDDO, OUTPUT);
    digitalWrite(LEDDO, LOW);


    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the certificate file (optional) */
    // config.cert.file = "/cert.cer";
    // config.cert.file_storage = StorageType::FLASH;

    /* Assign the database URL(required) */
    config.database_url = DATABASE_URL;

    config.signer.test_mode = true;

    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Nhiet do: ");
  Serial.print(t);
  Serial.print("*C  ");
  Serial.print("Do am: ");
  Serial.print(h);
  Serial.println("%  ");


  //gui cac tin hieu nhiet do va do am len firebase
  Firebase.setFloat( fbdo,"Nhiet do", t);
  Firebase.setFloat ( fbdo,"Do am", h);

  delay(200);

  //doc tin hieu analog cua cam bien khi gas
  A_value = analogRead(MQ2_A);

  
  if (millis() - dataMillis > 5000)
  {
    Serial.printf("Analog Value: %d\n", A_value);
    Serial.printf("Digital Value: %d\n", D_value);

    //gui gia tri tin hieu analog khi gas len firebase
    Serial.printf("Update ... %s\n", Firebase.setInt(fbdo, "/data/value", A_value) ? "Ok" : fbdo.errorReason().c_str());
    dataMillis = millis();
  }

  // if(!D_value){
  //   digitalWrite(LED, HIGH);
  //   delay(200);
  //   digitalWrite(LED, LOW);
  //   delay(200);
  // }
  if(2000<A_value){                         //neu gia tri analog cua khi gas tra ve lon hon 2000, led sang
    digitalWrite(LEDVANG, HIGH);
    delay(200);
    digitalWrite(LEDVANG, LOW);
    delay(200);
  }
    if( t > 40){                            //neu gia tri nhiet do tra ve lon hon 40, led sang
    digitalWrite(LEDDO, HIGH);
    delay(200);
    digitalWrite(LEDDO, LOW);
    delay(200);
  }
    if( h > 70){                            //neu gia tri do am tra ve lon hon 70, led sang
    digitalWrite(LEDXANH, HIGH);
    delay(200);
    digitalWrite(LEDXANH, LOW);
    delay(200);
  }
}






