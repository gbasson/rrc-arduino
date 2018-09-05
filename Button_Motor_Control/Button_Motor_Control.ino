#include <SoftwareSerial.h>
#include <AFMotor.h>
AF_DCMotor motor1(3, MOTOR12_64KHZ); // create motor #2, 64KHz pwm


// First we'll set up constants for the pin numbers.
// This will make it easier to follow the code below.

const int buttonPin1 = 22;  // pushbutton 1 pin
const int buttonPin2 = 24;  // pushbutton 2 pin


 

void setup()
{
  // Set up the pushbutton pins to be an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  pinMode(buttonPin1, INPUT_PULLUP); // Enable internal pull-up resistor on buttonPin
  pinMode(buttonPin2, INPUT_PULLUP); // Enable internal pull-up resistor on buttonPin
  
  //Setup all the Arduino Pins
    Serial.begin(9600); // set up Serial library at 9600 bps
    Serial.println("Motor test!");
    motor1.setSpeed(200); // set the speed to 200/255
   
  
}


void loop()
{
  // if the switch is high, motor will turn on one direction:
    if (digitalRead(buttonPin1) == LOW) {
      motor1.run(FORWARD); // turn it on going forward
      Serial.print("tick");
  }
//  else {
//      Serial.print("tack");
//      motor1.run(RELEASE); // stopped
//  }
  
    // if the switch is high, motor will turn on one direction:
    else if (digitalRead(buttonPin2) == LOW) {
      Serial.print("tock");
      motor1.run(BACKWARD); // the other way
  }
  else {
    Serial.print("tack");
    motor1.run(RELEASE); // stopped
  }
  
}
