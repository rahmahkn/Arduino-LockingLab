#include <Servo.h>

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
int failed = 0;
Servo servo;

void setup() {
  Serial.begin(9600);
  pinMode(bulb, OUTPUT);
  pinMode(button, INPUT);
  pinMode(sensor, INPUT);
  servo.attach(A0, 500, 2500);
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
    Serial.write(opened, 8);
    door_open = true;
    servo.attach(A0, 500, 2500);
    servo.write(179);
    
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
      servo.attach(A0, 500, 2500);
      servo.write(1);
      
      entering = false;
      exiting = false;
    } else {
      int durationOpen = (millis() - startOpen) / 1000;

      if (durationOpen > OPEN_TIME) {
      door_open = false;
      servo.attach(A0, 500, 2500);
      servo.write(1);
        
      entering = false;
        exiting = false;
        failed ++;
      }
    }
  }
  
  delay(100);
}