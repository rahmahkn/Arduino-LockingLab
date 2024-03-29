char opened[10] = "Opened";
char unlocked[10]; //Initialized variable to store recieved data
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

void setup() {
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
    Serial.flush();
  }
  
  if (digitalRead(button) == HIGH) {
    Serial.println("DOOR OPENED");
    Serial.write(opened, 6);
    door_open = true;
    
    exiting = true;
    entering = false;
  }
  
  Serial.println("Num person");
  Serial.println(num_person);
    
  if (door_open) {
    int valSensor = digitalRead(sensor);

    if (valSensor == HIGH) {
      if (entering) {
       if (num_person == 0) {
          digitalWrite(bulb, HIGH);
        }
        num_person ++;
      }
      
      if (exiting) {
        num_person --;
        if (num_person == 0) {
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
      }
    }
  }
  
  delay(100);
}