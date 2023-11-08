#include <FirebaseESP32.h>
#include <DHT.h>
#include  <WiFi.h>

#define FIREBASE_HOST "https://hc-sr04-341be-default-rtdb.firebaseio.com/" //Địa chỉ của Firebase Realtime Database
#define FIREBASE_AUTH "cMX6BX9vQB988IgNXRm82wbfewl891X3DgE5ASZu" //Khóa xác thực Firebase để truy cập vào dự án
#define WIFI_SSID "Duchung" // Thay đổi tên wifi của bạn
#define WIFI_PASSWORD "bo031203" // Thay đổi password wifi của bạn

//Cài đặt chân esp32  
int trigger_pin = 4;  
int echo_pin   = 18;
int distance_cm; 

FirebaseData fbdo; //Đối tượng FirebaseData dùng để tương tác với Firebase Realtime Database.

void setup() {
  Serial.begin(9600); //Bắt đầu kết nối Serial để theo dõi thông tin qua cổng nối tiếp với tốc độ baud 9600
  delay(1000);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);//Kết nối ESP32 với mạng Wi-Fi sử dụng tên mạng và mật khẩu được cung cấp
  Serial.print("Dang ket noi");
  while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  delay(500);
  }
  Serial.println ("");
  Serial.println ("Da ket noi WiFi!"); //hiển thị lên màn hình đã kết nối wifi khi kết nối thành công
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);//Firebase Realtime Database cũng được khởi tạo bằng Firebase.begin() với URL và mật khẩu Firebase được định nghĩa trước đó.
}

void loop() {
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  long duration = pulseIn(echo_pin, HIGH);
  distance_cm = (duration / 2) / 29.09;
  if (isnan(distance_cm) ) {
  Serial.println("Failed to read from HC_SR04 sensor!");
  }
  // hiển thị kết quả Height: x cm với x là khoảng cách đo được từ cảm biến
  Serial.print("Height: ");
  Serial.print(distance_cm);
  Serial.println("cm");

  Firebase.setFloat ( fbdo,"Height", distance_cm);

  delay(500);
}                                         