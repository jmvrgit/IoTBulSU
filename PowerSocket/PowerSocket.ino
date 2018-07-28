#include <SerialESP8266wifi.h>
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <SoftwareSerial.h>

//Pin configuration for Internet of Things Project
//Arduino  Module Pin  
//------- --------------------
//A0    Proximity Pin Out
//A1    
//A2    
//A3    
//A4    
//A5    
//A6    
//A7    
//D2    Power Analyzer TX
//D3    Power Analyzer RX
//D4    ESP8266/NodeMCU RX
//D5    ESP8266/NodeMCU RST
//D6    ESP8266/NodeMCU TX
//D7    
//D8
//D9    MFRC522/RFID RST
//D10   MFRC522/RFID SDA
//D11   MFRC522/RFID MOSI
//D12   MFRC522/RFID MISO
//D13   MFRC522/RFID SCK

//Pins are configured for Arduino Nano
#define proximityPin 0
#define relayPin 1
#define poweranalyzer_tx 2
#define poweranalyzer_rx 3
#define wifi_rx 4
#define wifi_rst 5
#define wifi_tx 6
#define mfrc522_RST 9
#define mfrc522_SDA 10

//Configure RFID Interface
MFRC522 mfrc522(mfrc522_SDA, mfrc522_RST);

//Configure Software Serials to be used on Digital Pins
SoftwareSerial poweranalyzer(poweranalyzer_rx, poweranalyzer_tx);
SoftwareSerial wifiSerial(wifi_rx, wifi_tx);

//disabled due to reconnection issue
//creating new ESP8266 object
//SerialESP8266wifi wifi(wifiSerial, wifiSerial, wifi_rst, Serial); //adding Serial enabled local echo and wifi debug

//MFRC522 Variables
String UID_card = ""; // set to empty to avoid unexpected characters
String stringTemp = ""; // same here
byte readCard[4]; // MFRC522 has 4 bytes (8 Characters)

//Wifi Variables
String wifiSSID = "jonmarco";
String wifiPASS = "jonmarco11";
String raspiIP = "192.168.254.100"; //needs to be const char* instead of string to work
String raspiPORT = "2121";

//WiFi Data to be Sent
String powerData;

String inputString;
boolean stringComplete = false;
unsigned long nextPing = 0;

void setup() {
  // Power socket initial Setup
  // Different Serial Processes needs to have different baud rate to be recognized
  poweranalyzer.begin(9600);
  Serial.begin(19200);
  wifiSerial.begin(9600);

  Serial.println("Serial baudrate: SET");
  //Configure to listen to devices
  wifiSerial.listen();
  poweranalyzer.listen();
  Serial.println("Serial listen set to ON");
  
  poweranalyzer.print("\002M4\003"); //“\002”=STX, “\003”=ETX
  Serial.println("Power Analyzer set to MODE 4");

  //configure pins for Digital output and analog input
  pinMode(relayPin, OUTPUT);
  pinMode(proximityPin, INPUT);

  Serial.println("Relay and Proximity Pins SET");
  
  //begin SPI interface for MFRC522
  SPI.begin();
  mfrc522.PCD_Init(); //Initialize MFRC522 Hardware
  //mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);

  Serial.println("RFID Initialized");
  
  //begin wifi interface for ESP8266/NodeMCU
  ATconnectToWifi();
  
  //wifi.setTransportToTCP();
  //wifi.endSendWithNewline(true);
  //wifi.begin();
  //wifi.connectToAP(wifiSSID, wifiPASS);
  //delay(200);
  //wifiSerial.println("AT+CIPMUX=1");
  //delay(2000);
  //wifi.connectToServer(raspiIP, raspiPORT);
 // Serial.println("WIFI SET");
 // wifi.send(SERVER, "WIFI set");
}

void loop() {
 // put your main code here, to run repeatedly:
 // if wifi is not started, turn it on

 long watthr;
 long volt;
 long amp;
 long power;
 
 volt = random(220, 240);
 amp = random(0, 10);
 power = random(10, 2400);
 watthr = random(0, 1200);

 String voltString = String (volt);
 String ampString = String (amp);
 String powerString = String (power);
 String watthrString = String (watthr);
 String message = powersenddata(voltString,ampString,powerString,watthrString);
 //Serial.println(message);
 }
}

String getID() {
  String nullString = "";
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continues
    return nullString;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return nullString;
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
  return UID_card;
}

void poweranalyzerfunc(){
  String watthr;
  String volt;
  String amp;
  String power;
  
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
    }
  }
}

void isPluggedin(){
  if(proximitySensor() > 200){
    Serial.println("No Appliance");
  }
  else if (proximitySensor() < 20){
    Serial.println("Appliance Plugged IN");
    String UID = getID();
    if (UID != ""){
      //send UID to Raspberry Pi for PHP/SQL execution
    }
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

String powersenddata(String volt, String amp, String power, String watthr){
 String message = "[" + volt + "||" + amp + "||" + power + "||" + watthr + "]\n";
 String commandSend = "AT+CIPSEND=1," + String(message.length());
 wifiSerial.println(commandSend); //Send to ID 1, length DATALENGTH
 delay(2000);
 wifiSerial.println(message); // Print Data
 delay(2000);
 return message;
}

void ATconnectToWifi(){
  Serial.println("Connecting to Wifi using AT Commands");
  wifiSerial.println("AT+CWMODE=1");//set to STA mode (Station mode); 1 = Station mode, 2= Access Point, 3 = Both
  delay(2000);
  String CWJAPString = "AT+CWJAP=\"" + wifiSSID + "\",\"" +wifiPASS+"\"";
  wifiSerial.println(CWJAPString);// Set SSID and Password
  delay(7000);
  wifiSerial.println("AT+CIPMUX=1"); // Set multiple connections to ON
  delay(2000);
  String CIPSTARTString = "AT+CIPSTART=1,\"TCP\",\"" + raspiIP + "\"\," + raspiPORT;
  wifiSerial.println(CIPSTARTString);
  delay(3000);
  wifiSerial.println("AT+CIPSEND=1,19"); // Send to ID = 1, Length 18
  delay(1000);
  wifiSerial.println("WIFI_CONNECTION_OK"); // Length of this string is 18
  delay(1000);
}

