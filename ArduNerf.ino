#include <Servo.h>

Servo ESC1;
Servo ESC2;
Servo feedServo;
//arm esc
// constants won't change. They're used here to 
// set pin numbers:
const int spinButtonPin = 8;    // the number of the pushbutton pin
const int feedButtonPin = 7;
const int ledPin = 13;      // the number of the LED pin
const int ESC1Pin = 5;      //piuninumber of the first ESC
const int ESC2Pin = 6;     //pin number of the second ESC
const int feedServoPin=9;
// Variables will change:
int pos = 0;              //Sets position variable
int ledState = LOW;         // the current state of the output pin
int spinButtonState;             // the current reading from the spin input pin
int feedButtonState;             // the current reading from the feed input pin
int lastSpinButtonState = HIGH;   // the previous reading from the input pin
int lastFeedButtonState = HIGH;   // the previous reading from the input pin
// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
int ESCMinSpeed = 1000;   //this varies by ESC.  Min speed must be low enough to arm.  Refer to speec controller's documentation for this
int ESCMaxSpeed = 1800;   //this varries by ESC.  You may want to set it slightly lower than the max supported to avoid overheating issues

void setup() {
  pinMode(spinButtonPin, INPUT);
  pinMode(feedButtonPin, INPUT);
  pinMode(ledPin, OUTPUT);
 // set initial LED state
  digitalWrite(ledPin, ledState);
  //attach servos
  ESC1.attach(ESC1Pin);  
  ESC2.attach(ESC2Pin);
  feedServo.attach(feedServoPin); 
  //write minimum spped to ESCs so they arm
  ESC1.writeMicroseconds(ESCMinSpeed);
  ESC2.writeMicroseconds(ESCMinSpeed);
}

void loop() {
  // read the state of the switch into a local variable:
  int spinReading = digitalRead(spinButtonPin);
  int feedReading = digitalRead(feedButtonPin);
  // check to see if you just pressed the button 
      
      
      //debounce filtering code will go here
     
     
     //only change speed if button state has changed
      if (spinReading != lastSpinButtonState) {
        if (spinReading == LOW) {
           ledState = HIGH;
           ESC1.writeMicroseconds(ESCMaxSpeed);
           ESC2.writeMicroseconds(ESCMaxSpeed);
        }
       if (spinReading == HIGH) {
          ledState = LOW;
          ESC1.writeMicroseconds(ESCMinSpeed);
          ESC2.writeMicroseconds(ESCMinSpeed);
        }
      }

      //if feed button has been pressed, start pushing darts in
      if (feedReading != lastFeedButtonState) {
        if (feedReading == LOW) {
          feedServo.write(180);
          }
        if (feedReading == HIGH) {
          feedServo.write(0);
        }
      }
    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastSpinButtonState = spinReading;
    lastFeedButtonState = feedReading;
  
  // set the LED:
  digitalWrite(ledPin, ledState);

}
