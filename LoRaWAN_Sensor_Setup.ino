
#include <SoftwareSerial.h>

SoftwareSerial rhf(8,9); //RX,TX


//VOID SETUP___________________________________________//
void setup() 
Serial.begin(9600);
while (!Serial){
  ;
}

Serial.println("Started");

rhf.begin(9600);

delay(1000);

}

//VOID LOOP___________________________________________//
void loop() {

delay(1000);
Serial.println("ID");
rhf.println("AT+ID");
delay(30);
while (rhf.available()){
  String inData = rhf.readStringUntil('\n');
  Serial.println("Got response from RHF: " +inData);

Serial.println("Network Key");
rhf.println("AT+KEY=NWKSKEY, 2B7E151628AED2A6ABF7158282ABCDEF");
delay(30);
while (rhf.available()){
  String inData = rhf.readStringUntil('\n');
  Serial.println("Got response from RHF: " +inData);

Serial.println("App Key");
rhf.println("AT+KEY=APPSKEY, 2B7E151628AED2A6ABF7158282ABCDEF");
while (rhf.available()){
  String inData = rhf.readStringUntil('\n');
  Serial.println("Got response from RHF: " +inData);

Serial.print("Frequency and Datarate");
rhf.println("AT+CH=0, 915, DR0, DR5");
while (rhf.available()){
  String inData = rhf.readStringUntil('\n');
  Serial.println("Got response from RHF: " +inData);

  
delay (10000000);
}
}
}
}
}
