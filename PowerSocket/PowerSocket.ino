#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>

#define relayPin 12
#define proximityPin 0
#define poweranalyzer_rx 3
#define poweranalyzer_tx 2

SoftwareSerial poweranalyzer(poweranalyzer_rx, poweranalyzer_tx);
String Data;
String watthr;
String volt;
String amp;
String power;
String acs;

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
}

void loop() {
  // put your main code here, to run repeatedly:
  poweranalyzerfunc();
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
  }
  else if (proximitySensor() < 20){
    Serial.println("Appliance Plugged IN");
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
