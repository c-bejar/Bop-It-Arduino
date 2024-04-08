/*

  Arduino Uno R3 - Bop-It!
  CS 362 Project

  Central Hub of the Bop-It Game
  You can choose player 1 and player 2 here
  You can start a game here
  You can determine a winner based on inputs and outputs received

*/


// include the library code
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
const int buttonPin =2;
const int buzzerPin = 7;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonState = 0;  // variable for reading the pushbutton status
bool startedGame = false; // variable used to determine if game started

unsigned long startTime = 0;
unsigned long currTime = 0;

void playBuzz(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
}

void gameCountdown() {
  int time = 3;
  while(true)
  {
    lcd.clear();
    //print out countdown on LCD
    lcd.setCursor(0,0);
    lcd.print(" START GAME IN");
    lcd.setCursor(0,1);
    lcd.print("       " + String(time));
    delay(1000); //should 1000, for one second
    time--;
    playBuzz(1500, 100);
    if(time < 0){playBuzz(2500, 500); break;} //1500 freq
    //print countdown and break out of loop
  }
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  startTime = millis();
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if(startedGame) {
    gameCountdown();
    while(true) {
      analogWrite(A0, LOW);
      lcd.clear();
      lcd.print("WE Gaming");
      
    }
  } else {
    lcd.setCursor(0,0);
    lcd.print("Start Game?");
    lcd.setCursor(0,1);
    lcd.print("Press Start");
    
    if(buttonState == HIGH)
    {
      startedGame = !startedGame;
    }
  }
}