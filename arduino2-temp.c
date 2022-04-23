char unlocked[10]; //Initialized variable to store recieved data
int bulb = 3;
int button = 2;

int sensor = 4;
int valSensor = 0;
int state = LOW;

void setup() {
  // Begin the Serial at 9600 Baud
  pinMode(bulb, OUTPUT);
  pinMode(button, INPUT);
  pinMode(sensor, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.readBytes(unlocked,8); //Read the serial data and store in var
  Serial.println(unlocked); //Print data on Serial Monitor
  
  if (strcmp(unlocked, "Unlocked")) {
    digitalWrite(bulb, HIGH);
  }
  
  valSensor = digitalRead(sensor);
  if (valSensor == HIGH) {
    digitalWrite(bulb, HIGH);
    delay(500);
    
    if (state == LOW) {
      Serial.println("Motion detected!");
      state = HIGH;
    }
  } else {
      digitalWrite(bulb, LOW); // turn LED OFF
      delay(500);             // delay 200 milliseconds 
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }
  
  delay(1000);
}