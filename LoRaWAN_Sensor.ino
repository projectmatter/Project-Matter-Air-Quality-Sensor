#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_SHT31.h>

//variables
float PM25;
float PM10;
String p;
String PMString25;
String PMString10;
int CSPIN = 10;
File dataFile;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

SoftwareSerial rhf(8,9); //RX, TX
SoftwareSerial sds011 (3,6);//RX,TX


//VOID SETUP___________________________________________//

void setup() {
Serial.begin(9600);
while (!Serial){
  ;
}

Serial.println("Started");

rhf.begin(9600);
sds011.begin(9600);
sht31.begin(0x44);
delay(1000);

SD.begin (CSPIN);
pinMode (CSPIN, OUTPUT);

}

//VOID LOOP___________________________________________//

void loop() {
  //SDS011
sds011.listen();
  while (sds011.read() != 171){
  }
  int data[10];
  for (int i = 0; i<= 9; i++){
    while (true) {
      if (sds011.available() > 0) {
        data[i] = sds011.read();
        break;
      }
    }
  }
 if (data[0] == 170 and data[9] == 171 and (data[2] + data[3] + data[4] + data[5] + data[6] + data[7]) % 256 == data[8]) {
    PM25 = (data[3] * 256 + data[2]) * 0.1;
    PM10 = (data[5] * 256 + data[4]) * 0.1;

Serial.println("PM2.5 = "); Serial.println(PM25);
Serial.println("PM10 = "); Serial.println(PM10);

//***********************************//

  //SHT31
int t = round(sht31.readTemperature());
int h = round(sht31.readHumidity());

Serial.println ("Temp = "); Serial.println(t);
Serial.println ("Hum = "); Serial.println(h);

//***********************************//

//NO2
  int no2 = 0;
  no2 = analogRead(A6);
  Serial.println("NO2: "); Serial.println(no2);

//***********************************//

  //SD Card
  //dataFile = SD.open("PM25.txt", FILE_WRITE);
  //dataFile.println(PM25);
  //dataFile.close();
  
  //dataFile = SD.open("PM10.txt", FILE_WRITE);
  //dataFile.println(PM10);
  //dataFile.close();
  
  //dataFile = SD.open("Temperature.txt",FILE_WRITE);
  //dataFile.println(t); dataFile.print("*C");
  //dataFile.close();
  
  //dataFile = SD.open("Humidity.txt",FILE_WRITE);
  //dataFile.println(h); dataFile.print("%");
  //dataFile.close();
  
  //dataFile = SD.open("NO2.txt",FILE_WRITE);
  //dataFile.println(no2);
  //dataFile.close();

//***********************************//

  //RHF76
delay(1000);
Serial.println("Sending an AT Command...");
PMString25 = String (PM25*10, 0);
PMString10 = String (PM10*10, 0);
p = "p";
rhf.println("AT+MSG=" +PMString25 +p +PMString10 +p +h +p +t +p +no2);

delay (2000);
}
}
