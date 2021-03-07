/*
  Arduino based Fencing Scoring detection
  https://github.com/Vilda007/ArduinoFencingScoringDetection

  Wiring:
  Fencer 1 hit detection (contact) - PIN 2 (+pulldown 100 kOhm)
  Fencer 2 hit detection (contact) - PIN 3 (+pulldown 100 kOhm)
  Fencer 1 hit signal LED (yellow) - PIN 5 (+pullup 330 Ohm)
  Fencer 2 hit signal LED (blue)   - PIN 6 (+pullup 330 Ohm)
  Ready LED (green)                - PIN 7 (+pullup 330 Ohm)
  Buzzer                           - PIN 8
  8x8 LED matrix display with MAX7219
    CLK                            - PIN 13 
    CS/LOAD                        - PIN 3
    DIN (Data IN)                  - PIN 11
    GND                            - GND
    VCC                            - +5 V

  This code has been heavily inspired by the examples you can find at
  https://www.arduino.cc/en/Tutorial/HomePage
*/

// Including the required Arduino libraries
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// 8x8 LED display driver type hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Definings
#define MAX_DEVICES 1 //Number of 8x8 LED displays
#define CS_PIN 3 

// constants won't change. They're used here to set pin numbers:
const int Fencer1 = 2;                  // Fencer 1 detection PIN
const int Fencer2 = 4;                  // Fencer 2 detection PIN
const int Fencer1LED = 5;               // Fencer 1 hit signal LED PIN
const int Fencer2LED = 6;               // Fencer 2 hit signal LED PIN 
const int ReadyLED = 7;                 // Green ready LED PIN
const int BuzzerPin = 8;                // Buzzer PIN
const char Fencer1HitSign[2] = ">>";    // Fencer 1 hit sign for display
const char Fencer2HitSign[2] = "<<";    // Fencer 2 hit sign for display
const int Fencer1HitSoundHz = 440;      // Fencer 1 hit sound pitch in Hz
const int Fencer2HitSoundHz = 880;      // Fencer 2 hit sound pitch in Hz
const int HitSoundDuration = 500;       // Hit sound duration in ms
const int HitDisplayedDuration = 2000;  // Hit sound duration in ms
const int ReadySoundHz = 220;           // Ready sound pitch in Hz
const int ReadySoundDuration = 50;      // Ready sound duration in ms
const int ScrollSpeed =  200;           // Speed of scrolling in ms

// variables will change:
int Fencer1hit = 0;                     // Fencer 1 scores
int Fencer2hit = 0;                     // Fencer 2 scores
int Fencer1hits = 0;                    // Fencer 1 score (hit counter)
int Fencer2hits = 0;                    // Fencer 2 score (hit counter)
char Score[10]  = "0:0";                // Score to display

// Create a new instance of the MD_Parola class with hardware SPI connection
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
  // open the serial port at 9600 bps:
  Serial.begin(9600); 
  
  // initialize the LED pins as an output:
  pinMode(Fencer1LED, OUTPUT);
  digitalWrite(Fencer1LED, HIGH);
  pinMode(Fencer2LED, OUTPUT);
  digitalWrite(Fencer2LED, HIGH);
  pinMode(ReadyLED, OUTPUT);
  digitalWrite(ReadyLED, LOW);
  
  // initialize the pushbutton pin as an input:
  pinMode(Fencer1, INPUT);
  pinMode(Fencer2, INPUT);

  // Intialize the object for 8x8 LED display
  myDisplay.begin();

  // Set the intensity (brightness) of the 8x8 LED display (0 min - 15 max)
  myDisplay.setIntensity(3);

  // Clear the 8x8 LED display
  myDisplay.displayClear();
  myDisplay.displayScroll("0:0", PA_CENTER, PA_SCROLL_LEFT, ScrollSpeed);
}

//function Hit is called after the hit is detected
void Hit(int FencerLED, char FencerHitSign[], int FencerHitSoundHz, int F1hits, int F2hits) { 
  digitalWrite(ReadyLED, HIGH);
  digitalWrite(FencerLED, LOW);
  tone(BuzzerPin, FencerHitSoundHz, HitSoundDuration);
  int HitDisplayed = HitDisplayedDuration / 4;
  myDisplay.displayReset();
  myDisplay.displayClear();
  myDisplay.setTextAlignment(PA_CENTER);
  myDisplay.print(FencerHitSign);
  delay(HitDisplayed);
  myDisplay.setInvert(true);
  myDisplay.print(FencerHitSign);
  delay(HitDisplayed);
  myDisplay.setInvert(false);
  myDisplay.print(FencerHitSign);
  delay(HitDisplayed);
  myDisplay.setInvert(true);
  myDisplay.print(FencerHitSign);
  delay(HitDisplayed);
  myDisplay.setInvert(false);
  tone(BuzzerPin, ReadySoundHz, ReadySoundDuration);
  myDisplay.displayClear();
  (String(F1hits) + ":" + String(F2hits)).toCharArray(Score,10);
  Serial.println("--");
  Serial.println(Score);
  myDisplay.displayScroll(Score, PA_CENTER, PA_SCROLL_LEFT, ScrollSpeed);
  digitalWrite(FencerLED, HIGH);
  digitalWrite(ReadyLED, LOW);
}

void loop() {
  // read the state of the pushbutton value:
  Fencer1hit = digitalRead(Fencer1);
  Fencer2hit = digitalRead(Fencer2);

  // Fencer 1 scores 
  if (Fencer1hit == HIGH) { 
    Fencer1hits = Fencer1hits + 1;
    Hit(Fencer1LED, Fencer1HitSign, Fencer1HitSoundHz, Fencer1hits, Fencer2hits);
  }

  // Fencer 2 scores 
  if (Fencer2hit == HIGH) { 
    Fencer2hits = Fencer2hits + 1;
    Hit(Fencer2LED, Fencer2HitSign, Fencer2HitSoundHz, Fencer1hits, Fencer2hits);
  }

  //scroll the actual score over the 8x8 LED display
  if (myDisplay.displayAnimate()) {
    myDisplay.displayReset();
  }
}
