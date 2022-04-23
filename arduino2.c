char unlocked[10]; //Initialized variable to store recieved data
int bulb = 3;
int button = 2;

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
  pinMode(bulb, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  Serial.readBytes(unlocked, 8); //Read the serial data and store in var
  Serial.println(unlocked); //Print data on Serial Monitor
  
  if (strcmp(unlocked, "Unlocked")) {
    digitalWrite(bulb, HIGH);
  }
  delay(1000);
}