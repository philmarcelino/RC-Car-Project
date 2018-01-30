//////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2018 Phil Marcelino 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

const int MSG_START = 255;
int MotorID = 0;
int Speed = 0;
int Direction = 0;
int On = 0;

Servo myServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Motor controller Tester");

  AFMS.begin();  // create with the default frequency 1.6KHz

  myServo.attach(9);  // attaches the servo on pin 9 to the servo object.
}

void loop() {
  // put your main code here, to run repeatedly:

  // check the serial port to see if there are any characters.
  if (Serial.available() > 0)
  {
    // get to the start of the message.
    while (Serial.parseInt() != MSG_START)
    {
      Serial.print("waiting for input");
      // don't do anything.
    }

    // we're at the start of the message, so safe to start reading in parameters
    MotorID = Serial.parseInt();
    Speed = Serial.parseInt();
    Direction = Serial.parseInt();
    On = Serial.parseInt();    

    Serial.print("Motor ID = ");
    Serial.println(MotorID);
    Serial.print("Speed = ");
    Serial.println(Speed);
    Serial.print("Direction = ");
    Serial.println(Direction);
    Serial.print("On = ");
    Serial.println(On);

    if (MotorID == 0)
    {
      // need to update the motor state
      UpdateDCMotor();
    }
    else
    {
      // Update Servo Motor
      // need to update the servo motor...
      Serial.println("Running the servo test...");
      UpdateServoMotor();
    }
  }
}

void UpdateServoMotor()
{
  int pos =myServo.read();

  Serial.print("Current Servo position new = ");
  Serial.println(pos);
  
  if (On)
  {
    for (; pos <= 135; pos+=1)
    {
      Serial.print("Setting position to ");
      Serial.println(pos);
      myServo.write(pos);
      delay(15);
      
      // check to see that what we set is what we're reading back (boundary check)
      if (pos != myServo.read())
      {
        Serial.print("Breaking at upper bound of ");
        Serial.println(myServo.read());
        break;
      }
      Serial.print("     Reading position: ");
      Serial.println(myServo.read());
    }
    for(pos=135; pos>=45; pos -=1)
    {
      Serial.print("Setting position to ");
      Serial.println(pos);
      myServo.write(pos);
      delay(15);

      // check to see that what we set is what we're reading back (boundary check)
      if (pos != myServo.read())
      {
        Serial.print("Breaking at upper bound of ");
        Serial.println(myServo.read());
        break;
      }
      Serial.print("     Reading position: ");
      Serial.println(myServo.read());
    }

    myServo.write(90);
  }
}

void UpdateDCMotor()
{
  if (Direction == 0)
  {
    myMotor->run(FORWARD);
  }
  else if (Direction == 1)
  {
    myMotor->run(BACKWARD);
  }

  myMotor->setSpeed(Speed);
}

