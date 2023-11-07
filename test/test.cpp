#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#define FIREBASE_HOST "android-flutter-8e415-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "bZMt8bWQCQK7gfwxSwMI6YROwGVHOxHsoN07itAH"

#define WIFI_SSID "Dam Thanh"
#define WIFI_PASSWORD "0904279981"

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

#define flame_sensor 14  // Connected to GPIO 14 (D14) on ESP32
#define DELAY_PIN 16     // Connected to GPIO 16 (D16) on ESP32
#define RELAY 19 // Connected to GPIO 19
#define coi 15     // Connected to GPIO 15 (D15) on ESP32
#define AO_PIN 34  // ESP32's pin GPIO36 connected to AO pin of the MQ2 sensor


int flame_sensor_read;
int gasValue = 0;

void sent_state_fire(bool state_fire) {
  if (!Firebase.setBool(fbdo, "/state_fire", state_fire)) {
    Serial.println(fbdo.errorReason());
  }
}
void sent_state_gas(bool state_gas) {
  if (!Firebase.setBool(fbdo, "/state_gas", state_gas)) {
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
  Serial.begin(9600);
  pinMode(RELAY , OUTPUT);
  connectToWiFi();
  setupFirebase();
  pinMode(flame_sensor, INPUT);
  pinMode(AO_PIN, INPUT);


  pinMode(DELAY_PIN, OUTPUT);
  pinMode(coi, OUTPUT);
}

void loop() {
  flame_sensor_read = digitalRead(flame_sensor);  // Đọc dữ liệu từ cảm biến lửa trên GPIO 14
  gasValue = analogRead(AO_PIN);

  bool state_fire = false;
  bool state_gas = false;

  if (flame_sensor_read == 0) {
    state_fire = true;
    Serial.println("Lửa đang cháy");
  }

  if (gasValue > 2500) {
    state_gas = true;
    Serial.println("Khí gas vượt ngưỡng an toàn");
  }

  if (state_fire || state_gas) {
    digitalWrite(DELAY_PIN, LOW);
    digitalWrite(coi, LOW);
  } else {
    digitalWrite(DELAY_PIN, HIGH);
    digitalWrite(coi, HIGH);
  }

  sent_state_fire(state_fire);
  sent_state_gas(state_gas);

  Serial.println("===============================");

  delay(1000);
}
void conTrolLight(){
    bool stateLight = true;

 Firebase.getBool(fbdo , "/control", &stateLight);
 if (stateLight == true)
 {
    digitalWrite(RELAY, LOW);
    /* code */
 }
 else
 {
    digitalWrite(RELAY ,HIGH);
 }
 
 
    
}