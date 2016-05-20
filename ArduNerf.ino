//Arduino program to control a nerf blaster with brushless flywheels and a servo for feeding darts into the flywheels
//debounce code modifed from https://www.arduino.cc/en/Tutorial/Debounce
//LED is used fortesting without havin to hook up motors constantly
//The pushbuttons im using in this example are OSEPP Push Button Modules  http://osepp.com/products/sensors-arduino-compatible/push-button-module/
//ARduino controller used during this development is an Intel Curie powered Arduino 101 https://www.arduino.cc/en/Main/ArduinoBoard101 with an OSEPP SensorShield on top of it http://osepp.com/products/shield-arduino-compatible/sensor-shield/


#include <Servo.h> //nice and simple, we are only utilizing one library for this project.  Servo handles everything we need it to

//declare servo objects that will be used throughout the rest of the project.
Servo ESC1;
Servo ESC2;
Servo feedServo;
//arm esc
// constants won't change. They're used here to 
// set pin numbers:
const int spinButtonPin = 8;  // the number of the pin where the spin button is connected
const int feedButtonPin = 7;  // the number of the pin where the spin button is connected
const int ledPin = 13;      // the number of the LED pin
const int ESC1Pin = 5;      //piuninumber of the first ESC
const int ESC2Pin = 6;     //pin number of the second ESC
const int feedServoPin=9;


// Variables will change:
int ledState = LOW;         // the current state of the output pin
int spinButtonState;             // the current reading from the spin input pin
int feedButtonState;             // the current reading from the feed input pin
int lastSpinButtonState = HIGH;   // the previous reading from the input pin
int lastFeedButtonState = HIGH;   // the previous reading from the input pin
// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastSpinDebounceTime = 0;  // the last time the output spin pin was toggled
long lastFeedDebounceTime = 0;  // the last time the output feed pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

//if not all your ESCs are the same, you may need an additonal variable but that shouldnt be necessary here.
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

    if (spinReading != lastSpinButtonState) {
    // reset the debouncing timer
    lastSpinDebounceTime = millis();
     }

      
  if ((millis() - lastSpinDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

   
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
  }

    if (feedReading != lastFeedButtonState) {
    // reset the debouncing timer
     lastFeedDebounceTime = millis();
     }

      
  if ((millis() - lastSpinDebounceTime) > debounceDelay) {
      //if feed button has been pressed, start pushing darts in
      if (feedReading != lastFeedButtonState) {
        //if button is pressed, feed a dart in
        if (feedReading == LOW) {
          feedServo.write(180);
          }
        //return servo to the withdrawn state when button is released  
        if (feedReading == HIGH) {
          feedServo.write(0);
        }
      }
  } 
  
    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastSpinButtonState = spinReading;
    lastFeedButtonState = feedReading;
  
  // set the LED:
  digitalWrite(ledPin, ledState);

}
