/*
  Arduino based Fencing Scoring detection
  https://github.com/Vilda007/ArduinoFencingScoringDetection
  

  This code has been heavily inspired by the examples you can find at
  https://www.arduino.cc/en/Tutorial/HomePage
*/

// constants won't change. They're used here to set pin numbers:
const int Fencer1 = 2;     // Fencer 1 detection PIN
const int Fencer2 = 3;     // Fencer 3 detection PIN
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int Fencer1hit = 0;         // Fencer 1 scores
int Fencer2hit = 0;         // Fencer 2 scores

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(Fencer1, INPUT);
  pinMode(Fencer2, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  Fencer1hit = digitalRead(Fencer1);
  Fencer2hit = digitalRead(Fencer2);

  if (Fencer1hit == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    tone(8,440,500);
    delay(1000);
    tone(8,440,10);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
