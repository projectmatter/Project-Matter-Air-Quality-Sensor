#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <SD.h>


int counter = 1;
float PM25;
float PM10; 

#define DHTPIN            6
#define DHTTYPE           DHT22
int CSPIN = 4; 
File dataFile;
String ug = " µg/m^3";

DHT_Unified dht(DHTPIN, DHTTYPE);


void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(9600);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  SD.begin(CSPIN);
  pinMode(CSPIN, OUTPUT);
  delay(100);
  //SD.remove("PM25.TXT");
  //SD.remove("PM10.TXT");
  //SD.remove("HUMIDITY.TXT");
  //SD.remove("TEMP.TXT");
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
    Serial.print("PM Data Unsucessful");
    while (Serial.read() != 171) {
    }
  }
  
  //HUMIDITY AND TEMP
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }

  //SD CARD
  Serial.println(PM25);
  dataFile = SD.open("PM25.txt", FILE_WRITE);
  dataFile.print("Data number: "); dataFile.println(counter);
  dataFile.print("PM2.5: "); dataFile.print(PM25); dataFile.println(ug);
  dataFile.println("");
  dataFile.close();
  dataFile = SD.open("PM10.txt", FILE_WRITE);
  dataFile.print("Data number: "); dataFile.println(counter);
  dataFile.print("PM10: "); dataFile.print(PM10); dataFile.println(ug);
  dataFile.println("");
  dataFile.close();
  dataFile = SD.open("Temp.txt", FILE_WRITE);
  dataFile.print("Data number: "); dataFile.println(counter);
  dataFile.print("Temperature: "); dataFile.print(event.temperature); dataFile.println(" *C");
  dataFile.println("");
  dataFile.close();
  dataFile = SD.open("Humidity.txt", FILE_WRITE);
  dataFile.print("Data number: "); dataFile.println(counter); 
  dataFile.print("Humidity: "); dataFile.print(event.relative_humidity); dataFile.println("%");
  dataFile.println(""); 
  dataFile.close();
  counter++;

  delay(5000);
}
