#include <LiquidCrystal.h> //include LCD library (standard library)
#include <Keypad.h> //include keypad library - first you must install library (library link in the video description)

unsigned long DELAY_TIME = 15; // 15 sec
unsigned long delayStart = 0; // the time the delay started
bool delayRunning = false; // true if still waiting for delay to finish

char* password ="1234"; //create a password
int pozisyon = 0; //keypad position

const byte rows = 4; //number of the keypad's rows and columns
const byte cols = 4;

char keyMap [rows] [cols] = { //define the cymbols on the buttons of the keypad
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte rowPins [rows] = {7, 6, 5, 4}; //pins of the keypad
byte colPins [cols] = {3, 2, 1, 0};

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, rows, cols );

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup()
{
    lcd.begin(16, 2); 
    // setLocked (true); //state of the password
}

void loop()
{
    if (!delayRunning) {
        lcd.setCursor(0, 0);
        lcd.print("Welcome");
        lcd.setCursor(0, 1);
        lcd.print("to Lab!");
    }

    char whichKey = myKeypad.getKey(); //define which key is pressed with getKey
	Serial.println(whichKey);
  
    if (whichKey == 'A' && !delayRunning) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter Password");
        
        delayStart = millis();
        delayRunning = true;
    }

    int secs = (millis() - delayStart) / 1000;

    if (delayRunning) {
        if (secs <= DELAY_TIME) {
            lcd.setCursor(0, 1);
            lcd.print(secs);
        } else {
            lcd.clear();
            delayRunning = false;
        }
    }

  	if (whichKey == '*' || whichKey == '#' || whichKey == 'B' || whichKey == 'C' || whichKey == 'D') {
        pozisyon=0;
        // setLocked (true);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  Invalid Key!");
        delay(1000);
        lcd.clear();
    }
  
    if (whichKey == password [pozisyon]) {
        pozisyon ++;
    }
    
    if (pozisyon == 4) {
        delayRunning = false;
        // setLocked (false);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("UNLOCKED");
        delay(3000);
        lcd.clear();
    }
    delay(100); 
}

// void setLocked(int locked){
//     if (locked) {
//         lcd.setCursor(0, 0);
//         lcd.print("Locked");
//         lcd.setCursor(0, 1);
//         lcd.print("");
//     }
//     else {
//         lcd.setCursor(0, 0);
//         lcd.print("Unlocked");
//         lcd.setCursor(0, 1);
//         lcd.print("");
//     }
// }