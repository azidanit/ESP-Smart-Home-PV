//1x4 Keypad from Adafruit.com (Unofficial Source) 
//Original Source: Button
//Referenced Tutorial on Arduino.com: http://arduino.cc/en/Tutorial/InputPullupSerial
//Referenced: http://arduino.cc/en/Reference/pinMode
//
//Turns on and off specified Pin or Prints to Serial Monitor when keys are press on a 1x4 keypad from Adafruit.com
//
//The (OLD) circuit:
//* LED attached from pin 13 to ground
//* pushbutton attached to pin 2 from +5V
//* pushbutton attached to pin 3 from +5V
//* 10K resistor attached to pin 2 from ground
//
//The (NEW) circuit (2014):
//
//* Whichever component you want attached to Pin 13 (e.g- LED)
//* 1x4 Keypad w/ primary pin attached to Ground and 1-4 keys attached to PINS 9-12
//* Serial Monitor used for output (for testing, when not using anything attached to Pin 13) 
//
//* Note: 
// 
// 
//
//created 2005
//by DojoDave 
//modified 30 Aug 2011
//by Tom Igoe
//modified 10 Apr 2012
//by mpilchfamily
//modified 8 Dec 2014 
//by C Cunningham

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin[] = {27,14,25,26};     // the number of the pushbutton pins
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
//  pinMode(ledPin, OUTPUT);  
  // initialize the Serial Monitor @ 9600
  Serial.begin(115200);  
  // initialize the keypad pin(s) as an input:
  for(int x=0; x<4; x++)
  {
    pinMode(buttonPin[x], INPUT_PULLUP); 
  }  
}

void loop(){
  // read the state of the keypad value:
  for(int x=0; x<4; x++)
  {
    //signifying the state of which the button is in by reading the appropriate pin #
    buttonState = digitalRead(buttonPin[x]);
    if (buttonState == LOW){
      Serial.print("PRESSED ");
      Serial.print(x+1);
      Serial.print(" ");
    Serial.println(buttonPin[x]);  
    }
    
  }
}