//Used adafruit libraries licensed under MIT license

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

//Definitions
#define Wifi_SSID       "SSID"
#define Wifi_PASS       "password"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "AIO Username"
#define AIO_KEY         "AIO KEy"

WiFiClient client;
Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish PM25_Pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pm25");
Adafruit_MQTT_Publish PM10_Pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pm10");
Adafruit_MQTT_Publish humidity_Pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish temperature_Pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish no2_pub = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/no2");

void setup() {
  Serial.begin(9600);
  sht31.begin(0x44);
  delay(1000);
  Serial.println("Project Matter PM Device V3");
  WiFi_connect();
//  mqtt.subscribe(&state);
}

void loop() {
  MQTT_connect();
  //sleep(); for reading values from adafruit
  bool pm = true;
  while(pm){
    pm = PM_data();
  }
  dht();
  no2();
  delay(2500);
}

void WiFi_connect(){
  Serial.print("Connecting to ");
  Serial.println(Wifi_SSID);
  WiFi.begin(Wifi_SSID, Wifi_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
bool PM_data(){
  float PM25;
  float PM10;
  while (Serial.read() != 171) {
    }
  int data[10];
  for (int i = 0; i <= 9; i++) {
    while (true) {
      if (Serial.available() > 0) {
        data[i] = Serial.read();
        break;
      }
    }
  }
  if (data[0] == 170 and data[9] == 171 and (data[2] + data[3] + data[4] + data[5] + data[6] + data[7]) % 256 == data[8]) {
    PM25 = (data[3] * 256 + data[2]) * 0.1;
    PM10 = (data[5] * 256 + data[4]) * 0.1;
    Serial.print("Sending PM25 value: "); Serial.println(PM25);
    PM25_Pub.publish(PM25);
    Serial.print("Sending PM10 value: "); Serial.println(PM10); 
    PM10_Pub.publish(PM10);
    return false;
  }
  else{
    Serial.println("PM Data Unsucessful");
    return true;
  }
}
//void sleep(){
//  Adafruit_MQTT_Subscribe *subscription;
//  while ((subscription = mqtt.readSubscription(5000))) {
//    if (subscription == &state) {
//      Serial.print(F("Got: "));
//      Serial.println((char *)state.lastread);
//    }
//  }
//}

void dht(){
  float humidity = sht31.readHumidity();
  float temperature = sht31.readTemperature();
  if (! isnan(temperature)) {  
    Serial.print("Temp *C = "); Serial.println(temperature);
    temperature_Pub.publish(temperature);
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(humidity)) { 
    Serial.print("Hum. % = "); Serial.println(humidity);
    humidity_Pub.publish(humidity);
  } else { 
    Serial.println("Failed to read humidity");
  }
}

void no2(){
  float no2 = 0;
  no2 = analogRead(A6);
  Serial.print("NO2: "); Serial.println(no2);
  no2_pub.publish(no2);
}
