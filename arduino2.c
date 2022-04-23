char unlocked[10]; //Initialized variable to store recieved data

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
}

void loop() {
  Serial.readBytes(unlocked,5); //Read the serial data and store in var
  Serial.println(unlocked); //Print data on Serial Monitor
  delay(1000);
}