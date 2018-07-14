#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <SoftwareSerial.h>

#define relayPin 2
#define proximityPin 0
#define poweranalyzer_rx 3
#define poweranalyzer_tx 2

MFRC522 mfrc522(10, 9);
SoftwareSerial poweranalyzer(poweranalyzer_rx, poweranalyzer_tx);

String Data;
String watthr;
String volt;
String amp;
String power;
String acs;

String UID_card;
String stringTemp;
byte readCard[4];

void setup() {
  // put your setup code here, to run once:

  //Serial Setup
  // Different Serial Processes needs to have different baud rate to be recognized
  poweranalyzer.begin(9600);
  Serial.begin(19200);
  
  poweranalyzer.listen();
  poweranalyzer.print("\002M4\003"); //“\002”=STX, “\003”=ETX
  
  pinMode(relayPin, OUTPUT);
  pinMode(proximityPin, INPUT);

  SPI.begin();
  mfrc522.PCD_Init(); //Initialize MFRC522 Hardware
  //mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("wat");
  getID();
  delay(1000);
}

int getID() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continues
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
  UID_card = "";
  for (int i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    stringTemp = String(readCard[i],HEX);
    UID_card = UID_card + stringTemp;
  }
  mfrc522.PICC_HaltA(); // Stop reading
  Serial.println(UID_card);
}

void poweranalyzerfunc(){
    if (poweranalyzer.available()>0) {
     if (poweranalyzer.find("Volt")){
      volt = poweranalyzer.parseFloat();
      Serial.print("Voltage: ");
      Serial.println(volt);
    }
       if (poweranalyzer.find("Amp")){
      amp = poweranalyzer.parseFloat();
      Serial.print("Current: ");
      Serial.println(amp);
    }
       if (poweranalyzer.find("Watt")){
      power = poweranalyzer.parseFloat();
      Serial.print("Current: ");
      Serial.println(power);
    }
     if (poweranalyzer.find("Watt-Hr")){
      watthr = poweranalyzer.parseFloat();
      Serial.print("Watt Hours: ");
      Serial.println(watthr);
      //sendpowerdata();
    }
  }
}

void isPluggedin(){
  if(proximitySensor() > 200){
    Serial.println("No Appliance");
    relayOff();
  }
  else if (proximitySensor() < 20){
    Serial.println("Appliance Plugged IN");
    relayOn();
  }
}

int proximitySensor(){
  int proximity_value = analogRead(proximityPin);
  return proximity_value;
}

void relayOn(){
  digitalWrite(relayPin, 0);
}

void relayOff(){
  digitalWrite(relayPin, 1);
}
