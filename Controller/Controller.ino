void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  String readString;
  String Q;

  while(Serial1.available()) {
    delay(1);
    if (Serial1.available() > 0) {
      char c = Serial1.read();
      Serial.println(c);
      if (isControl(c)) {
        break;
      }
    }
  }

  while (Serial.available()) {
    delay(1);
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (isControl(c)) {
        break;
      }

      readString += c;
    }
  }
    
  Q = readString;

  if (Q = "on") {
    Serial1.print("1");
    Serial.println("Sent: On");
  }
  if (Q = "off") {
    Serial1.print("2");
    Serial.println("Sent: Off");
  }

  
}