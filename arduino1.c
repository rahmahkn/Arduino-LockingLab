#include <LiquidCrystal.h> //include LCD library (standard library)
#include <Keypad.h> //include keypad library - first you must install library (library link in the video description)
#include <Servo.h>

unsigned long DELAY_TIME = 10; // 15 sec
unsigned long DELAY_INPUT = 10;
unsigned long delayRunningStart = 0; // the time the delay started
unsigned long delayInputStart = 0;
bool delayRunning = false; // true if still waiting for delay to finish
bool delayInput = false;

char* password ="1234"; //create a password
int pozisyon = 0; //keypad position
int digits = 0;

const byte rows = 4; //number of the keypad's rows and columns
const byte cols = 4;
char nums [] = "1234567890";

char keyMap [rows] [cols] = { //define the cymbols on the buttons of the keypad
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte rowPins [rows] = {7, 6, 5, 4}; //pins of the keypad
byte colPins [cols] = {3, 2, A2, A3};

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols );

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

Servo servo;
char unlocked[10] = "Unlocked";

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2); 
    servo.attach(A0, 500, 2500);
}

void loop()
{
    if (!delayRunning && !delayInput) {
        lcd.setCursor(0, 0);
        lcd.print("Welcome");
        lcd.setCursor(0, 1);
        lcd.print("to Lab!");
        servo.write(1);
    }

    if (delayInput) {
        lcd.setCursor(0, 0);
        lcd.print("Try again in");
        int secsDelay = (millis() - delayInputStart) / 1000;

        if (secsDelay <= DELAY_INPUT) {
            lcd.setCursor(0, 1);
            lcd.print(DELAY_INPUT - secsDelay);
        } else {
            lcd.clear();
            delayInput = false;
        }
    }

    char tempWhichKey = myKeypad.getKey(); //define which key is pressed with getKey
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
            lcd.print("   ");
            lcd.setCursor(0, 1);
            lcd.print(DELAY_TIME - secs);
        } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Time is Up!");
            lcd.clear();
            delayRunning = false;
            pozisyon = 0;
        }
    }

    bool cond1 = (whichKey == '*' || whichKey == '#' || whichKey == 'B' || whichKey == 'C' || whichKey == 'D');
    bool cond2 = strchr(nums, whichKey);

  	if (cond1 && !delayInput) {
        pozisyon = 0;
        digits = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Invalid Input!");
        delay(100);
        lcd.clear();
    }
  
    if (!delayInput && delayRunning) {
        if (whichKey == password [pozisyon]) {
            lcd.setCursor(10+digits, 0);
            lcd.print("*");
            delay(100);
            pozisyon ++;
            digits ++;
        } else if (cond2) {
            lcd.setCursor(10+digits, 0);
            lcd.print("*");
            delay(100);
            digits ++;
        }
        
        if (digits == 4 && pozisyon == 4) {
            delayRunning = false;
            pozisyon = 0;
            digits = 0;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("UNLOCKED");
            Serial.write(unlocked, 8);

            servo.attach(A0, 500, 2500);
            servo.write(179);
            delay(100);
            lcd.clear();
        } else if (digits == 4 && pozisyon < 4) {
            delayRunning = false;
            delayInput = true;
            delayInputStart = millis();
            pozisyon = 0;
            digits = 0;
            lcd.clear();
            lcd.setCursor(0, 0);

            lcd.print("INCORRECT PASSWORD");
            servo.attach(A0, 500, 2500);
            servo.write(1);
            delay(100);
            lcd.clear();
        }
    }

    delay(100); 
}