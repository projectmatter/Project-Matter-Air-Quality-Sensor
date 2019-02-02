#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <SD.h>

//variables
int counter = 1;
float PM25;
float PM10;
float h;
float t; 
#define DHTPIN            6
#define DHTTYPE           DHT22
File dataFile;
String ug = " µg/m^3";
bool PMOnly = true;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  delay(2000);
  
  Serial.begin(9600);
  dht.begin();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor! Sensor has been set to PM only");
    PMOnly = true;
  }
  else{
    Serial.println("Successfully initialized! Temperature and Humidity set");
    PMOnly = false;
  }
  pinMode(4, OUTPUT);
  SD.begin(4);
  delay(1000);
}
void loop() {   
  //PARTICULATE MATTER
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
    Serial.println("");
    Serial.print("Data number: "); Serial.println(counter);
    Serial.print("PM2.5: "); Serial.print(PM25);
    Serial.println(ug); Serial.print("PM10: ");
    Serial.print(PM10); Serial.println(ug);
  }
  else{
    Serial.println("PM Data Unsucessful");
    while (Serial.read() != 171) {
    }
  }

  
  //HUMIDITY AND TEMP
  // Get temperature event and print its value.
  if (PM10 != 0){
    if (PMOnly == false){
    h = dht.readHumidity();
    t = dht.readTemperature();
    Serial.print("Temperature: "); Serial.println(t);
    Serial.print("Humidity: "); Serial.println(h);
    dataFile = SD.open("TEMP.TXT", FILE_WRITE);
    dataFile.println(t);
    dataFile.close();
    dataFile = SD.open("HUMIDITY.TXT", FILE_WRITE);
    dataFile.println(h);
    dataFile.close();
  }
  
  //SD CARD
  dataFile = SD.open("PM25.TXT", FILE_WRITE);
  dataFile.println(PM25);
  dataFile.close();
  dataFile = SD.open("PM10.TXT", FILE_WRITE);
  dataFile.println(PM10);
  dataFile.close();
  }
  
  
  counter++;
  delay(5000);
}

void deleteFiles(){
  SD.remove("PM25.TXT");
  SD.remove("PM10.TXT");
  SD.remove("HUMIDITY.TXT");
  SD.remove("TEMP.TXT");
}
