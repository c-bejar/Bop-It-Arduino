/*

  Arduino Uno R3 - Bop-It!
  CS 362 Project
  Team 15

  Central Hub of the Bop-It! Game
  You can start a game here
  You can determine a winner based on inputs and outputs received

*/

// include the library code
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// declare rx/tx pins
#define rxPin 5
#define txPin 6

int roundLength = 5000; // milliseconds
const int addScore = 25;      // score to add to player with correct input
const int negScore = 10;      // score to subtract to player with incorrect input
const int nonScore = 25;      // score to subtract to player with no input
const int maxScore = 500;     // score needed for any given player to win
const int diffScore = 100;    // score difference needed for any given player to win

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
const int buttonPin = 2;
const int buzzerPin = 7;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonState = 0;  // for reading the pushbutton status
bool startedGame = false; // to determine if game started

// to be used for concurrent execution of code
unsigned long startTime = 0;

int roundDelay = 1000;  // delay between rounds
int roundNumber = 1;    // counts number of rounds

// to determine whether or not player one and two sent input
bool receivedOne = false;
bool receivedTwo = false;

// player scores
int p1Score = 0;
int p2Score = 0;

// set up software serial
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

char receivedSoftware() {
  if (mySerial.available()) {
    return char(mySerial.read());
  }
  return 'F';
}

char receivedHardware() {
  if (Serial.available()) {
    return char(Serial.read());
  }
  return 'F';
}

// test to see if it can do both
// might work
char receivedData(Stream& serialPort) {
 if (serialPort.available() > 0) {
    return char(serialPort.read());
 }
 return 'F';
}

//
//  newRound(int round)
//
//  displays onto the LCD the current round number and
//  counts down 3 2 1 . . .
//
void newRound(int round) {
  int time = 3;
  roundNumber++;
  while (true) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    Round " + String(round) + " In");
    lcd.setCursor(0,1);
    lcd.print("       " + String(time));
    time--;
    delay(roundDelay);
    playBuzz(1500, 100);
    if(time < 0) {playBuzz(2500, 100); break;}
  }
}

//
//  printLCD()
//
//  displays onto the LCD the current minigame, as well
//  as the current scores of each player.
//
void printLCD(String game, int p1Score, int p2Score) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(game);
  lcd.setCursor(0, 1);
  lcd.print("P1: " + String(p1Score) + " P2: " + String(p2Score));
}

//
//  determineGame(int game)
//
//  displays onto the LCD the current round's minigame.
//  Takes a look at 0-7 to determine which game was selected.
//
String determineGame(int game) {
  switch (game) {
    case 0:
      return "Press Red!";
    case 1:
      return "Press Green!";
    case 2:
      return "Press Blue!";
    case 3:
      return "Flick Up!";
    case 4:
      return "Flick Down!";
    case 5:
      return "Flick Left!";
    case 6:
      return "Flick Right!";
    case 7:
      return "Bop It!";
    default:
      return "Who Knows Bro";
  }
}

//
//  compileScores(char p1Input, char p2Input, int currGame)
//
//  changes player scores depending on input received.
//  Also returns true/false if win by score difference.
//
void compileScores(char p1Input, char p2Input, int currGame) {
  char currentGame = char(currGame);
  // change score values
  if (currentGame == p1Input)
    p1Score += addScore;
  else if (p1Input == 'F')
    p1Score -= nonScore;
  else
    p1Score -= negScore;
  if (currentGame == p2Input)
    p2Score += addScore;
  else if (p2Input == 'F')
    p2Score -= nonScore;
  else
    p2Score -= negScore;
}

//
//  determineWinner()
//
//  Determine Winner whether by max score or by
//  difference in score. Returns 1 or 2 depending
//  on who wins (1 = player 1 and 2 = player 2).
//
int determineWinner() {
  // tied scores
  if (p1Score == p2Score) {
    p1Score -= maxScore;
    p2Score -= maxScore;
    return 0; // should this result in a tie game, or sub scores and continue?
  // one player is >= max score, so insta win
  } else if (p1Score >= maxScore || p2Score >= maxScore) {
    if (p1Score > p2Score) {
      return 1;
    }
    return 2;
  // player wins by difference in scores
  } else if (p1Score - p2Score >= diffScore) {
    return 1;
  } else if (p2Score - p1Score >= diffScore) {
    return 2;
  }
  return 0; // game continues?
}

//
//  gameLoop()
//
//  The main loop for the game when it's being played.
//  Looped through this function until the game ends and
//  a player is chosen to win.
//
void gameLoop() {
  while(true) {
    char p1Input = '0'; // TEMPORARY UNTIL WE CAN GET COMMUNICATION WORKING
    char p2Input = '0'; // TEMPORARY UNTIL WE CAN GET COMMUNICATION WORKING
    // determine minigame to be played
    int game = random(0, 8); // 0 - 7
    // get string for current minigame
    String currentMinigame = determineGame(game);
    startTime = millis();
    while(millis() - startTime <= roundLength) {
      // print to screen relevant info
      printLCD(currentMinigame, p1Score, p2Score);
      //  TODO: send out command and get data from players
      //        using p1Input and p2Input
      //  TODO+: Add score and break out of loop
      // WE MIGHT WANT TO CALL THE BELOW FUNCTION FROM RECEIVED DATA FUNCTIONS
      // compileScores(p1Input, p2Input, game);
    }
    int won = determineWinner();
    //  TODO: add win/loss screen to players if someone wins
    if (won == 1) {
      // player 1 wins
      startedGame = false;
      break;
    } else if (won == 2) {
      // player 2 wins
      startedGame = false;
      break;
    }
    // displays current round and timer
    newRound(roundNumber);
  }
}

// ################################################################### SETUP
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // set inputs and outputs
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  // SOFTWARE SERIAL
  // set up rx/tx pins as input/output
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  Serial.begin(9600);
}

// #################################################################### LOOP
void loop() {
  buttonState = digitalRead(buttonPin);
  
  if(startedGame) {
    gameCountdown();  // counts 3 2 1 . . .
    gameLoop();       // main game loop where gameplay happens
  // Before game is started
  } else {
    lcd.setCursor(0,0);
    lcd.print("   Start Game?  ");
    lcd.setCursor(0,1);
    lcd.print("   Press Start  ");
    
    // Button pressed
    if(buttonState == HIGH)
    {
      startedGame = true; // start game
    }
  }
}