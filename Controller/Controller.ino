//used for Arduino serial connection
#include <SoftwareSerial.h>

#define rxPin 13
#define txPin 12

//ultrasonic sensor pins
const int triggerPin = 4;
const int echoPin = 3;
//push Pins
const int pinRed = 10;
const int pinGreen = 9;
const int pinBlue = 8;
const int pushPin1 = 7;
const int pushPin2 = 6;
const int pushPin3 = 5;

// joystick pin
const int swPin = 11;
const int vrxPin = A4;
const int vryPin = A5;

int xVal;
int yVal;

float distance;
unsigned long duration;

char redButton = '0';
char greenButton = '1';
char blueButton = '2';
char flickUp = '3';
char flickDown = '4';
char flickRight = '5';
char flickLeft = '6';
char bopIt = '7';

unsigned long startTime;
const unsigned long interval = 3000;

SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup() {
   // ultrasonic distance thing
   pinMode(triggerPin, OUTPUT);
   pinMode(echoPin, INPUT);

   // LED pins
   pinMode(pinRed, OUTPUT);
   pinMode(pinGreen, OUTPUT);
   pinMode(pinBlue, OUTPUT);

   //button pins
   pinMode(pushPin1, INPUT);
   pinMode(pushPin2, INPUT);
   pinMode(pushPin3, INPUT);

   // Set input/output pins
  //Serial monitor set up
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);

}

void updateLED(char cmd) {
  if(cmd == '0') {
    digitalWrite(pinRed, HIGH);
    digitalWrite(pinGreen, LOW);
    digitalWrite(pinBlue, LOW);
  } else if(cmd == '1') {
    digitalWrite(pinRed, LOW);
    digitalWrite(pinGreen, HIGH);
    digitalWrite(pinBlue, LOW);
  } else if(cmd == '2') {
    digitalWrite(pinRed, LOW);
    digitalWrite(pinGreen, LOW);
    digitalWrite(pinBlue, HIGH);
  } else {
    digitalWrite(pinRed, LOW);
    digitalWrite(pinGreen, LOW);
    digitalWrite(pinBlue, LOW);
  }
}

String checkCmd(int distance) {
  if(digitalRead(pushPin1) == HIGH) {
    return "2";
  }
  if(digitalRead(pushPin2) == HIGH) {
    return "1";
  }
  if(digitalRead(pushPin3) == HIGH) {
    return "0";
  }
  // read analog X and Y analog values
  xVal = analogRead(vrxPin) / 4;
  yVal = analogRead(vryPin) / 4;

  if(xVal == 0) {
    return "3";
  }
  if(xVal == 255) {
    return "4";
  }
  if(yVal == 0) {
    return "6";
  }
  if(yVal == 255) {
    return "5";
  }
  if(distance <= 5.0) {
    return "7";
  }
  return "U";
}
char receivedData() {
 if (mySerial.available()) {
    return char(mySerial.read());
 }
 return 'F';
}

float readSensor() {
  // Clears the trigPin
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  return duration * 0.034 / 2;
}
bool waiting = true;
String receive;
void loop() {
  if(waiting) {
    if (mySerial.available()) {
      receive = mySerial.readString();
      Serial.println("received: " + receive);
      waiting = false;
    }
    startTime = millis();
    return;
  } else {
    updateLED(receive[0]);
    float data = readSensor();
    String cmd = checkCmd(data);
    if(cmd != "U") {
      Serial.println(cmd);
      mySerial.print(cmd);
      waiting = true;
    }
    if(millis() - startTime > interval) {
      mySerial.print(cmd);
      waiting = true;
    }
  }
}
// //used for Arduino serial connection
// #include <SoftwareSerial.h>

// #define rxPin 13
// #define txPin 12

// //ultrasonic sensor pins
// const int triggerPin = 4;
// const int echoPin = 3;
// //push Pins
// const int pinRed = 10;
// const int pinGreen = 9;
// const int pinBlue = 8;
// const int pushPin1 = 7;
// const int pushPin2 = 6;
// const int pushPin3 = 5;

// // joystick pin
// const int swPin = 11;
// const int vrxPin = A4;
// const int vryPin = A5;

// int xVal;
// int yVal;

// float distance;
// unsigned long duration;

// char redButton = '0';
// char greenButton = '1';
// char blueButton = '2';
// char flickUp = '3';
// char flickDown = '4';
// char flickRight = '5';
// char flickLeft = '6';
// char bopIt = '7';

// unsigned long startTime;
// const unsigned long interval = 3000;

// // SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

// void setup() {
//    // ultrasonic distance thing
//    pinMode(triggerPin, OUTPUT);
//    pinMode(echoPin, INPUT);

//    // LED pins
//    pinMode(pinRed, OUTPUT);
//    pinMode(pinGreen, OUTPUT);
//    pinMode(pinBlue, OUTPUT);

//    //button pins
//    pinMode(pushPin1, INPUT);
//    pinMode(pushPin2, INPUT);
//    pinMode(pushPin3, INPUT);

//    // Set input/output pins
//   //Serial monitor set up
//   // pinMode(rxPin, INPUT);
//   // pinMode(txPin, OUTPUT);
//   // mySerial.begin(9600);
//   Serial.begin(9600);

// }

// String checkCmd(int distance) {
//   if(digitalRead(pushPin1) == HIGH) {
//     return "2";
//   }
//   if(digitalRead(pushPin2) == HIGH) {
//     return "1";
//   }
//   if(digitalRead(pushPin3) == HIGH) {
//     return "0";
//   }
//   // read analog X and Y analog values
//   xVal = analogRead(vrxPin) / 4;
//   yVal = analogRead(vryPin) / 4;

//   if(xVal == 0) {
//     return "5";
//   }
//   if(xVal == 255) {
//     return "6";
//   }
//   if(yVal == 0) {
//     return "4";
//   }
//   if(yVal == 255) {
//     return "3";
//   }
//   if(distance <= 5.0) {
//     return "7";
//   }
//   return "U";
// }
// char receivedData() {
//  if (Serial.available()) {
//     return char(Serial.read());
//  }
//  return 'F';
// }

// float readSensor() {
//   // Clears the trigPin
//   digitalWrite(triggerPin, LOW);
//   delayMicroseconds(5);
//   // Sets the trigPin on HIGH state for 10 micro seconds
//   digitalWrite(triggerPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(triggerPin, LOW);
//   // Reads the echoPin, returns the sound wave travel time in microseconds
//   duration = pulseIn(echoPin, HIGH);
//   // Calculating the distance
//   return duration * 0.034 / 2;
// }
// void updateLED(char cmd) {
//   if(cmd == '0') {
//     digitalWrite(pinRed, HIGH);
//     digitalWrite(pinGreen, LOW);
//     digitalWrite(pinBlue, LOW);
//   } else if(cmd == '1') {
//     digitalWrite(pinRed, LOW);
//     digitalWrite(pinGreen, HIGH);
//     digitalWrite(pinBlue, LOW);
//   } else if(cmd == '2') {
//     digitalWrite(pinRed, LOW);
//     digitalWrite(pinGreen, LOW);
//     digitalWrite(pinBlue, HIGH);
//   } else {
//     digitalWrite(pinRed, LOW);
//     digitalWrite(pinGreen, LOW);
//     digitalWrite(pinBlue, LOW);
//   }
// }
// bool waiting = true;
// String receive;
// void loop() {
//   if(waiting) {
//     if (Serial.available()) {
//       receive = Serial.readString();
//       // Serial.println("received: " + receive);
//       waiting = false;
//     }
//     startTime = millis();
//     return;
//   } else {
//     updateLED(receive[0]);
//     float data = readSensor();
//     String cmd = checkCmd(data);
//     if(cmd != "U") {
//       // Serial.println(cmd);
//       Serial.print(cmd);
//       waiting = true;
//       updateLED("");
//     }
//     if(millis() - startTime > interval) {
//       Serial.print(cmd);
//       waiting = true;
//       updateLED("");
//     }
//   }
// }