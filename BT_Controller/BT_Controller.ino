/*
*  Arduino, HM-10, App Inventor 2.0
*/ 

#include <SoftwareSerial.h>
SoftwareSerial BTserial(53, 51); // RX-51, TX-53

#include <AFMotor.h>
  AF_DCMotor motor1(1, MOTOR12_64KHZ); // create motor #1, 64KHz pwm
  AF_DCMotor motor2(2, MOTOR12_64KHZ); // create motor #2, 64KHz pwm
  AF_DCMotor motor3(3, MOTOR12_64KHZ); // create motor #3, 64KHz pwm
  AF_DCMotor motor4(4, MOTOR12_64KHZ); // create motor #4, 64KHz pwm

//Variables used for incoming data
  const byte maxDataLength = 20;
  char receivedChars[21] ;
  boolean newData = false;
  char BT_Type = 'N';
  int CurrentPin;
  int var;
 
//Constants for hardware
  const byte BUTTON_PIN[][2] = {{24,22}};
  const byte LED_PIN[][2] = {{30,32}};
  const int BUTTON_COUNT = 1;
 
//General variables
  boolean LED_State[BUTTON_COUNT][2];
  boolean Motor_State[BUTTON_COUNT][2];
 
//*********************************************************************************************************************
void setup()  
{
  // general variables
    for (int thisItem = 0; thisItem < BUTTON_COUNT; thisItem++) 
    {
      for (byte i = 0; i < BUTTON_COUNT; i++)
        { 
        LED_State[thisItem][i] = false;
        Motor_State[thisItem][i] = false;
        }
    }
  
    for (byte pin = 0; pin < BUTTON_COUNT; pin++) 
    {
       for (byte i = 0; i < 2; i++)
        { 
    // Set the button switch pins for input
      pinMode(BUTTON_PIN[pin][i], INPUT);
      pinMode(BUTTON_PIN[pin][i], INPUT_PULLUP); // Enable internal pull-up resistor on buttonPin
    
    // Set the LED pins for output and make them LOW
      pinMode(LED_PIN[pin][i], OUTPUT);  
      digitalWrite(LED_PIN[pin][i],LOW);
        }
    }
 
  //open serial communication for debugging
    Serial.begin(9600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
  //open software serial connection to the Bluetooth module.
    BTserial.begin(9600); 
    Serial.println("BTserial started at 9600");
    newData = false;

  //Set-up Motor variables.  
    motor3.setSpeed(200); // set the speed to 200/255
}
 
//********************************************************************************************************************* 
void loop()  
{
    for (byte BtnNum = 0; BtnNum < BUTTON_COUNT; BtnNum++) 
      {    ButtonPress(BtnNum); 
      }
    
    
    
    recvWithStartEndMarkers();               // check to see if we have received any new commands
    
    if (newData)  
      { processBTCommand(); 
        Serial.print("NEW BT Command");
      }   // if we have a new command do something about it
    // else { OnGoingCommand(); }
}

//*********************************************************************************************************************
void ButtonPress( byte pos)
{
  for (int thisItem = 0; thisItem < 2; thisItem++)
  {  
    //Check Button State for first button;
      switch (digitalRead(BUTTON_PIN[pos][thisItem])) {  //First Button
      
      case 0:  //Button pressed
        processAction( pos, thisItem, 0);
     //   digitalWrite(LED_PIN[pos][thisItem],LOW);
//        if (thisItem == 0) {
//          motor3.run(FORWARD); // the other way 
//        } else  if (thisItem == 1) {
//          motor3.run(BACKWARD); // the other way 
//        }
        
            Serial.print("BUTTON_PIN:");
            Serial.print(BUTTON_PIN[pos][thisItem]);
            Serial.print(" | ");
            Serial.print("BUTTON_VAL:");
            Serial.print(digitalRead(BUTTON_PIN[pos][thisItem]));
            Serial.print('\n');
        
        break;
      default:
        //If nothing else matches, do the default
        digitalWrite(LED_PIN[pos][thisItem],HIGH);
        motor3.run(RELEASE); // stopped

        break;
      }    
    }
}

//********************************************************************************************************************* 
void processAction( int btn, int pin, int state)
{
  //Change LED
    digitalWrite(LED_PIN[btn][pin],state);
  //Change Motor

    if (pin == 0) {
      motor3.run(FORWARD); // the other way 
    } else  if (pin == 1) {
      motor3.run(BACKWARD); // the other way 
    }
}



//********************************************************************************************************************* 
void processBTCommand()
{
      Serial.print("receivedChars = ");   Serial.println(receivedChars);

      byte LEDnum = receivedChars[1] - 48;          // convert ascii to value by subtracting 48
      boolean state = receivedChars[2] - 48;
      boolean Motorstatus = receivedChars[2] - 48; // convert ascii to value by subtracting 48 0-Forward 1-Reverse

      Serial.print("BT Type:");
      Serial.println(BT_Type); //************************************************
      
 //   LED_State[LEDnum-1] = LEDstatus;
      if (newData) {
        BT_Type = receivedChars[0];
      } else {
        BT_Type = BT_Type;
      }

      switch (BT_Type) {
        case 'T':   //Tap interaction
          var = 0;
          Serial.println("Tap"); //************************************************
          while(var < 150){
           processAction( LEDnum-1, 1, state);
            
            var++;
          }
          break;
        case 'P':  //Pull and hold interaction
          //do something when var equals P
          digitalWrite(LED_PIN[LEDnum-1][1],LOW);
          Serial.println("Pull and Hold"); //************************************************
          motor1.run(FORWARD); // stopped
          CurrentPin = LED_PIN[LEDnum-1][1];
          break;
        case 'R':  //Release interaction
          //do something when var equals 2
          digitalWrite(LED_PIN[LEDnum-1][1],LOW);
          motor1.run(RELEASE); // stopped
          break;
      }

    receivedChars[0] = '\0';
    newData = false;
}
 
//*********************************************************************************************************************
void recvWithStartEndMarkers()
{
     static boolean recvInProgress = false;
     static byte ndx = 0;
     char startMarker = '[';
     char endMarker = ']';
     char rc;
    
     if (BTserial.available() > 0) 
     {
      rc = BTserial.read();
      if (recvInProgress == true) 
      {
           if (rc != endMarker) 
           {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx > maxDataLength) { ndx = maxDataLength; }
           }
           else 
           {
                 receivedChars[ndx] = '\0'; // terminate the string
                 recvInProgress = false;
                 ndx = 0;
                 newData = true;
           }
      }
      else if (rc == startMarker) { recvInProgress = true; }
     }
}


//********************************************************************************************************************* 
void processBTCommand_old()
{
      Serial.print("receivedChars = ");   Serial.println(receivedChars);

    //Check Button States;
    //Value at pos 1 is the type of transaction
    //Value at pos 2 is the button to correct
    //Value at pos 3 is the button to correct
      byte LEDnum = receivedChars[1] - 48;          // convert ascii to value by subtracting 48
      boolean LEDstatus = receivedChars[2] - 48;
      boolean Motorstatus = receivedChars[2] - 48; // convert ascii to value by subtracting 48 0-Forward 1-Reverse

      Serial.print("BT Type:");
      Serial.println(BT_Type); //************************************************
      
 //   LED_State[LEDnum-1] = LEDstatus;
      if (newData) {
        BT_Type = receivedChars[0];
      } else {
        BT_Type = BT_Type;
      }

      switch (BT_Type) {
        case 'T':   //Tap interaction
          var = 0;
          Serial.println("Tap"); //************************************************
          while(var < 150){
            digitalWrite(LED_PIN[LEDnum-1][1],LOW);
            if (Motorstatus = 0) {
              motor1.run(FORWARD); // the other way
            } else {
              motor1.run(BACKWARD); // the other way
            }
            
            var++;
          }
          break;
        case 'P':  //Pull and hold interaction
          //do something when var equals P
          digitalWrite(LED_PIN[LEDnum-1][1],LOW);
          Serial.println("Pull and Hold"); //************************************************
          motor1.run(FORWARD); // stopped
          CurrentPin = LED_PIN[LEDnum-1][1];
          break;
        case 'R':  //Release interaction
          //do something when var equals 2
          digitalWrite(LED_PIN[LEDnum-1][1],LOW);
          motor1.run(RELEASE); // stopped
          break;
      }

    receivedChars[0] = '\0';
    newData = false;
}
