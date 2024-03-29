#include <LiquidCrystal.h> //include LCD library (standard library)
#include <Keypad.h> //include keypad library - first you must install library (library link in the video description)
#include <Servo.h>

unsigned long DELAY_TIME = 15; // 15 sec
unsigned long DELAY_INPUT = 10;
unsigned long delayRunningStart = 0; // the time the delay started
unsigned long delayInputStart = 0;
bool delayRunning = false; // true if still waiting for delay to finish
bool delayInput = false;
bool door_open = false;

char* password ="1234"; //create a password
int position = 0; //keypad position
int digits = 0;
int startOpen = 0;

//number of the keypad's rows and columns
const byte rows = 4;
const byte cols = 4;
char nums [] = "1234567890";

char keyMap [rows] [cols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//pins of the keypad
byte rowPins [rows] = {7, 6, 5, 4};
byte colPins [cols] = {3, 2, A4, A5};

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols );

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

Servo servo;
char unlocked[10] = "Unlocked";
char opened[10];

void setup()
{
    Serial.begin(1000);
    lcd.begin(16, 2); 
    servo.attach(A0, 500, 2500);
    servo.write(0);
}

void loop()
{
    if (Serial.available()) {
        Serial.readBytes(opened, 6); //Read the serial data and store in var
        startOpen = millis();
        door_open = true;
        Serial.println("DOOR OPENED");
        servo.write(90);
    }

    int durationOpen = (millis() - startOpen) / 1000;
    if (door_open && (durationOpen > DELAY_INPUT)) {
        servo.write(0);
        door_open = false;
    }

    if (!delayRunning && !delayInput) {
        lcd.setCursor(0, 0);
        lcd.print("    Welcome");
        lcd.setCursor(0, 1);
        lcd.print("     to Lab!");
    }
  
    if (delayInput) {
        lcd.setCursor(0, 0);
        lcd.print("  Try again in");
        int secsDelay = (millis() - delayInputStart) / 1000;

        if (secsDelay <= DELAY_INPUT) {
            lcd.setCursor(0, 1);
            lcd.print("      ");
            lcd.setCursor(6, 1);
            lcd.print("  ");
            lcd.setCursor(6, 1);
            lcd.print(DELAY_INPUT - secsDelay);
        } else {
            lcd.clear();
            delayInput = false;
        }
    }

    //define which key is pressed with getKey
    char tempWhichKey = myKeypad.getKey();
    char whichKey = ' ';
    if (tempWhichKey) {
        whichKey = tempWhichKey;
    }
  
    if (whichKey == 'A' && !delayRunning && !delayInput) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Password: ");
        
        delayRunningStart = millis();
        delayRunning = true;
    }

    int secs = (millis() - delayRunningStart) / 1000;

    if (delayRunning && !delayInput) {
        if (secs <= DELAY_TIME) {
            lcd.setCursor(0, 1);
            lcd.print("Input in");
            lcd.setCursor(9, 1);
            lcd.print("   ");
            lcd.setCursor(9, 1);
            lcd.print(DELAY_TIME - secs);
        } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("   Time is Up!");
            lcd.clear();
            delayRunning = false;
            position = 0;
        }
    }

    bool cond1 = (whichKey == '*' || whichKey == '#' || whichKey == 'B' || whichKey == 'C' || whichKey == 'D');
    bool cond2 = strchr(nums, whichKey);
  
    if (!delayInput && delayRunning) {
        if (whichKey == password [position]) {
            lcd.setCursor(10+digits, 0);
            lcd.print("*");
            delay(100);
            position ++;
            digits ++;
        } else if (cond2) {
            lcd.setCursor(10+digits, 0);
            lcd.print("*");
            delay(100);
            digits ++;
        }
        
        if (digits == 4 && position == 4) {
            delayRunning = false;
            position = 0;
            digits = 0;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("UNLOCKED");
            Serial.write(unlocked, 8);
            startOpen = millis();

          	door_open = true;
            servo.write(90);
            delay(100);
            lcd.clear();
        } else if (digits == 4 && position < 4) {
            delayRunning = false;
            delayInput = true;
            delayInputStart = millis();
            position = 0;
            digits = 0;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("    INCORRECT");
            lcd.setCursor(0, 1);
            lcd.print("    PASSWORD");
            delay(1000);
            lcd.clear();
        }
    }

    delay(100);
    Serial.flush();
}