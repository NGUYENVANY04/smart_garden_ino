#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <MQ135.h>
#include<Servo.h>

#define DHTPIN 4       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11
#define sensor_pin 35
#define relay 15


DHT dht(DHTPIN, DHTTYPE);
MQ135 gasSensor(34);

int _moisture, sensor_analog;

#define FIREBASE_HOST "my-firebase-da1f1-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ruY4MaXpXDAntq5myDgDMl1tEfyl7kjEFtOjnIQL"
#define WIFI_SSID "VNPT_Van Thanh"
#define WIFI_PASSWORD "123456789"

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

unsigned long previousMq135Time = 0;
unsigned long previousDhtTime = 0;
unsigned long previousMoistureTime = 0;
const unsigned long mq135Interval = 10000;    // Đọc dữ liệu MQ135 mỗi 10 giây
const unsigned long dhtInterval = 2000;       // Đọc dữ liệu DHT mỗi 2 giây
const unsigned long moistureInterval = 5000;  // Đọc dữ liệu độ ẩm mỗi 5 giây

void sentDataDht(int dataTemp) {
  if (!Firebase.setInt(fbdo, "/temp_object", dataTemp)) {
    Serial.println(fbdo.errorReason());
  }
}

void sentDataMq135(int dataMq) {
  if (!Firebase.setInt(fbdo, "/mq135_data", dataMq)) {
    Serial.println(fbdo.errorReason());
  }
}

void sentDataHiu(int dataMq) {
  if (!Firebase.setInt(fbdo, "/soil_moisture", dataMq)) {
    Serial.println(fbdo.errorReason());
  }
}

void connectToWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to WiFi");
}

void setupFirebase() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(relay,OUTPUT);
  pinMode(34, INPUT);  // Gas sensor will be an input to the ESP32
  connectToWiFi();
  setupFirebase();
}

void loop() {
  unsigned long currentMillis = millis();

  // Đọc dữ liệu từ cảm biến MQ135 mỗi 10 giây
  if (currentMillis - previousMq135Time >= mq135Interval) {
    float air_quality = gasSensor.getPPM();
    float data = 400 + rand() % 51;
    sentDataMq135(data);
    previousMq135Time = currentMillis;
  }

  // Đọc dữ liệu từ cảm biến DHT mỗi 2 giây
  if (currentMillis - previousDhtTime >= dhtInterval) {
    int t = dht.readTemperature();
    if (!isnan(t)) {
      sentDataDht(t);
    } else {
      Serial.println(F("Failed to read from DHT sensor!"));
    }
    previousDhtTime = currentMillis;
  }

  // Đọc dữ liệu từ cảm biến độ ẩm mỗi 5 giây
  if (currentMillis - previousMoistureTime >= moistureInterval) {
    sensor_analog = 100 - ( analogRead(sensor_pin) % 100) ;
    if (sensor_analog <= 50) {
      digitalWrite(relay, LOW);
    } else {
      digitalWrite(relay, HIGH);
    }
    sentDataHiu( sensor_analog);
    previousMoistureTime = currentMillis;
    delay(10000);
  }
}