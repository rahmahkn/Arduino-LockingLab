char unlocked[10]; //Initialized variable to store recieved data
char unlockedTemp[10];
int bulb = 3;
int button = 2;
int sensor = 4;
int startOpen = 0;
int OPEN_TIME = 10;
int num_person = 0;
bool door_open = false;
bool bulb_high = false;

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  pinMode(bulb, OUTPUT);
  pinMode(button, INPUT);
  pinMode(sensor, INPUT);
}

void loop() {
  if (Serial.available() && (num_person == 0)) {
    Serial.readBytes(unlocked, 8); //Read the serial data and store in var
    startOpen = millis();
    door_open = true;
    bulb_high = true;
  }
  
  if (door_open && bulb_high) {
    digitalWrite(bulb, HIGH);
    delay(1000);
    digitalWrite(bulb, LOW);
    bulb_high = false;
  }
  
  Serial.println(num_person);
    
  if (door_open) {
    int valSensor = digitalRead(sensor);
    Serial.println(valSensor);

    if (valSensor == HIGH) {
      num_person ++;
      door_open = false;
    } else {
      int durationOpen = (millis() - startOpen) / 1000;

      if (durationOpen > OPEN_TIME) {
      door_open = false;
      }
    }
  }
  
  // int valSensor = digitalRead(sensor);
  // if (valSensor == HIGH) {
  //   digitalWrite(bulb, HIGH);
  //   delay(1000);
  //   digitalWrite(bulb, LOW);
  //   delay(1000);
  // } else {
  //   digitalWrite(bulb, LOW);
  // }
  
  delay(100);
}