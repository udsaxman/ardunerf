//Arduino program to control a nerf blaster with brushless flywheels and a servo for feeding darts into the flywheels
//The pushbuttons im using in this example are OSEPP Push Button Modules  http://osepp.com/products/sensors-arduino-compatible/push-button-module/
//Arduino controller used during this development is an Intel Curie powered Arduino 101 https://www.arduino.cc/en/Main/ArduinoBoard101 with an OSEPP SensorShield on top of it http://osepp.com/products/shield-arduino-compatible/sensor-shield/.  You will need to use OPTO ESCs and a seperate power feed for this setup!
//Also tested wih an ATMEGA2560 based Multiwii Flight Controller i had laying around http://www.readytoflyquads.com/multiwii-pro-2-0-flight-controller.  To use this you will need to make sure one of your ESCs has a BEC built in.
//ESCs utilized here are http://www.hobbyking.com/hobbyking/store/__39708__Afro_ESC_30Amp_Multi_rotor_Motor_Speed_Controller_SimonK_Firmware_.html.  


#include <Servo.h> //nice and simple, we are only utilizing one library for this project.  Servo handles everything we need it to

//declare servo objects that will be used throughout the rest of the project.
Servo ESC1;
Servo ESC2;
Servo feedServo;
//arm esc
// constants won't change. They're used here to 
// set pin numbers:
const int spinButtonPin = 8;  // the number of the pin where the spin button is connected
const int feedButtonPin = 7;  // the number of the pin where the feed button is connected
const int ledPin = 13;      // the number of the LED pin
const int ESC1Pin = 5;      //piuninumber of the first ESC
const int ESC2Pin = 6;     //pin number of the second ESC
const int feedServoPin=9;  //pin number for the servo that will be pushing darts into the flywheels


// Variables will change:
int ledState = LOW;         // the current state of the output pin
int spinButtonState;             // used for storing the current reading from the spin input pin
int feedButtonState;             // used for storing the current reading from the feed input pin
int lastSpinButtonState = HIGH;   // the previous reading from the spin input pin - we instantiate it to the non-firing position to start the program
int lastFeedButtonState = HIGH;   // the previous reading from the feed input pin - we instantiate it to the non-firing position to start the program

//if not all your ESCs are the same, you may need an additonal variable but that shouldnt be necessary here.
int ESCMinSpeed = 1000;   //this varies by ESC.  Min speed must be low enough to arm.  Refer to speec controller's documentation for this
int ESCMaxSpeed = 1800;   //this varries by ESC.  You may want to set it slightly lower than the max supported to avoid overheating issues


void setup() {
  //setup the pins so we can use them for reading the button state.
  pinMode(spinButtonPin, INPUT);
  pinMode(feedButtonPin, INPUT);
  //set ledPin to output power to the LED
  pinMode(ledPin, OUTPUT);
  //set initial LED state
  digitalWrite(ledPin, ledState);
  //attach servos
  ESC1.attach(ESC1Pin);  
  ESC2.attach(ESC2Pin);
  feedServo.attach(feedServoPin); 
  //write minimum speed to ESCs so they arm
  ESC1.writeMicroseconds(ESCMinSpeed);
  ESC2.writeMicroseconds(ESCMinSpeed);
  //wait 1 second for ESCs to arm
  delay(1000);
}

void loop() {
  
  // read the state of the switchs into a local variable:
  int spinReading = digitalRead(spinButtonPin);
  int feedReading = digitalRead(feedButtonPin);
  
      // check to see if you just pressed the button 
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

      //if feed button has been pressed, start pushing darts out
      //What you write to the servos will vary greatly depending how you setup the feed mechanism.  
      //This example has the servo pushing a dart in when you push the button and then moving back to get ready to push the next one when yo let up on it.
      //In a rapidstrike or similar, you can just set ti to keep spinning while the button is pressed
      
      if (feedReading != lastFeedButtonState) {
        //if button is pressed, feed a dart in
        if (feedReading == LOW) {
          feedServo.write(180); //writes the server to full rotation
          }
        //return servo to the withdrawn state when button is released  
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
