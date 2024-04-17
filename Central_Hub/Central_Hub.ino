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
#include <SoftwareSerial.h>

// declare rx/tx pins
#define rxPin 5
#define txPin 6

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
const int buttonPin =2;
const int buzzerPin = 7;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonState = 0;  // for reading the pushbutton status
bool startedGame = false; // to determine if game started

unsigned long startTime = 0;
unsigned long currTime = 0;

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

// ######################################################### HELPER FUNCTIONS
//
//  playBuzz(frequency, duraction)
//
//  Plays a tone on the buzzer at specified frequency
//  and for the specified duration of time in milliseconds
//
void playBuzz(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
}

//
//  gameCountdown()
//
//  displays onto the LCD the countdown for the game to
//  start. Plays a tone on the buzzer as it does so.
//
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
    delay(1000); // one second
    time--;
    playBuzz(1500, 100);
    if(time < 0) {playBuzz(2500, 250); break;}
  }
}

int receivedSoftware() {
  if (mySerial.available() > 0)
  {
    String receive = mySerial.readStringUntil('\n');
    Serial.println("Received: " + receive);
    return receive.toInt();
  }
  return -1;
}

int receivedHardware() {
  if (Serial.available() > 0)
  {
    String receive = Serial.readStringUntil('\n');
    Serial.println("Received: " + receive);
    return receive.toInt();
  }
  return -1;
}

// ################################################################### SETUP
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // SOFTWARE SERIAL
  // set up rx/tx pins as input/output
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  Serial.begin(9600);

  startTime = millis();
}

// #################################################################### LOOP
void loop() {
  receivedHardware();
  return; // temporary return to test

  buttonState = digitalRead(buttonPin);
  
  if(startedGame) {
    gameCountdown();
    while(true) {
      lcd.clear();
      lcd.print("WE Gaming");
      delay(1000);
      startedGame = false;
      break;
    }
  // Before game is started
  } else {
    lcd.setCursor(0,0);
    lcd.print("   Start Game?");
    lcd.setCursor(0,1);
    lcd.print("   Press Start");
    
    // Button pressed
    if(buttonState == HIGH)
    {
      startedGame = true;
    }
  }
}