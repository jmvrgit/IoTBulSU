
int relayPin = 2;
int proximityPin =0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  pinMode(relayPin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  isPluggedin();
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
