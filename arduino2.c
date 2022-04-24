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
bool entering = false;
bool exiting = false;
int failed = 0;
//bool sent = false;

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  pinMode(bulb, OUTPUT);
  pinMode(button, INPUT);
  pinMode(sensor, INPUT);
}

void loop() {
  if (Serial.available()) {
    Serial.readBytes(unlocked, 8); //Read the serial data and store in var
    startOpen = millis();
    Serial.println("DOOR OPENED");
    door_open = true;
    entering = true;
    //sent = true;
    Serial.flush();
  }
  
//  if (door_open && bulb_high) {
  //  digitalWrite(bulb, HIGH);
   // delay(1000);
    //digitalWrite(bulb, LOW);
    //bulb_high = false;
 // }
  
  if (digitalRead(button) == HIGH){
    Serial.println("DOOR OPENED");
    door_open = true;
    exiting = true;
    entering = false;
  }
  
  Serial.println(num_person);
    
  if (door_open) {
    int valSensor = digitalRead(sensor);

    if (valSensor == HIGH) {
      if (entering) {
       if (num_person == 0) {
          digitalWrite(bulb, HIGH);
          delay(500);
          digitalWrite(bulb, LOW);
        }
        num_person ++;
      }
      
      if (exiting) {
        num_person --;
        if (num_person == 0) {
          digitalWrite(bulb, HIGH);
          delay(500);
          digitalWrite(bulb, LOW);
        }
      }
      
      door_open = false;
      entering = false;
      exiting = false;
    } else {
      int durationOpen = (millis() - startOpen) / 1000;

      if (durationOpen > OPEN_TIME) {
      door_open = false;
      entering = false;
        exiting = false;
        failed ++;
      }
    }
  }
  
  delay(100);
}