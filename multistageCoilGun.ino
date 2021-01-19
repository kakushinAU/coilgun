// This code has been tested on an ESP32 Arduino style board to operate a
// multistage coil gun.  After startup, you can either fire the coils by
// sending any character via a 115200 baud serial connection from a computer
// Alternatively, you can connect a manual pushbutton which momentarily grounds
// pin 34.  The ESP32 was programmed to operate at 240Mhz so that the 
// reaction time is as fast as possible (~6us). Have fun and be safe.

int sensor1 = 12;                                                         // set variable sensor1 as GPIO 12 on pin G12
int sensor2 = 14;                                                         // set variable sensor2 as GPIO 14 on pin G14
int trigger1 = 13;                                                        // set variable trigger1 as GPIO 33 on pin G33
int trigger2 = 32;                                                        // set variable trigger2 as GPIO 32 on pin G32
int fireButton = 34;                                                      // set variable fireButton as GPIO 34 on pin G34
int n = 1;                                                                // shot number
float velocity, energy, duration, speed1, speed2;                         // initiate variables for speed energy and duration
float mass = 0.013;                                                       // mass of the projectile in kilograms
float length = 0.050;                                                     // distance between speedometer sensors in meters
float stage1distance = .150;                                              // distance between start position and trigger 1 senor
float stage2distance = .160;                                              // distance between trigger 1 and trigger 2 sensors
volatile int flag1, flag2, flag3, flag4, flag5, fireEnable;               // flags for storing the state of sensors
volatile unsigned long mark1, mark2, mark3, mark4, mark5, mark2A, mark3A; // initialize timestamps of various events
volatile unsigned long timeElapsed, timeOfLastCheck;                      // initialize additional timestamp

int coil1 = 25; // set variable coil1 as GPIO 25 on pin G25
int coil2 = 26; // set variable coil2 as GPIO 26 on pin G26
int coil3 = 27; // set variable coil3 as GPIO 27 on pin G27

void IRAM_ATTR start() // Interrupt function: passed 1st speedometer sensor, remember time, set flag
{
  if (flag4 == 0)
  {
    mark4 = micros();
    flag4 = 1;
  }
}

void IRAM_ATTR finish() // Interrupt function: passed 2nd speedometer sensor, remember time, set flag
{
  if (flag5 == 0)
  {
    mark5 = micros();
    flag5 = 1;
  }
}

void IRAM_ATTR triggered1() // Interrupt function: passed trigger1, remember time, set flag
{
  if (flag2 == 0)
  {
    mark2 = micros();
    flag2 = 1;
  }
}

void IRAM_ATTR triggered2() // Interrupt function: passed trigger2, remember time, set flag
{
  if (flag3 == 0)
  {
    mark3 = micros();
    flag3 = 1;
  }
}

void setup()
{
  Serial.begin(115200);                          // open the COM port
  Serial.println("starting up");                 // start up message
  pinMode(sensor1, INPUT_PULLUP);                // configure pin input
  pinMode(sensor2, INPUT_PULLUP);                // configure pin input
  pinMode(trigger1, INPUT_PULLUP);               // configure pin input
  pinMode(trigger2, INPUT_PULLUP);               // configure pin input
  attachInterrupt(sensor1, start, RISING);       // set up interrupt pin to run function start
  attachInterrupt(sensor2, finish, RISING);      // set up interrupt pin to run function finish
  attachInterrupt(trigger1, triggered1, RISING); // set up interrupt pin to run function triggered1
  attachInterrupt(trigger2, triggered2, RISING); // set up interrupt pin to run function triggered2
  pinMode(coil1, OUTPUT);
  pinMode(coil2, OUTPUT);
  pinMode(coil3, OUTPUT);
  pinMode(fireButton, INPUT_PULLUP);
}

void loop()
{
  checkSensorFault();    // FUNCTION - reset all of the sensors and outputs every 2 seconds
  checkForProjectile();  // FUNCTION - Check if speedometer sensor flags triggered, then calculate speed
  checkForFireCommand(); // FUNCTION - Check for serial fire command from computer
  checkForFireButton();  // FUNCTION - Check for physical fire button being pressed (optional)
  checkForFlag2();       // FUNCTION - Check whether projectile has entered stage 2, fire stage 2
  checkForFlag3();       // FUNCTION - Check whether projectile has entered stage 3, fire stage 3
  delayMicroseconds(10); // delay timer for stability
}

void checkSensorFault()
{
  timeElapsed = micros() - timeOfLastCheck;
  if (timeElapsed > 2000000) // reset all sensors every 2 seconds
  {
    flag1 = 0;                // reset the sensors
    flag2 = 0;                // reset the sensors
    flag3 = 0;                // reset the sensors
    flag4 = 0;                // reset the sensors
    flag5 = 0;                // reset the sensors
    fireEnable = 0;           // disable firing the coils
    digitalWrite(coil1, LOW); // reset coil1
    digitalWrite(coil2, LOW); // reset coil2
    digitalWrite(coil3, LOW); // reset coil3
    timeOfLastCheck = micros();
  }
}

void checkForFlag2()
{
  if (flag2 == 1 && fireEnable == 1)
  {
    digitalWrite(coil2, HIGH);
    mark2A = micros();
    flag2 = 2;
  }
}

void checkForFlag3()
{
  if (flag3 == 1 && fireEnable == 1)
  {
    digitalWrite(coil3, HIGH);
    mark3A = micros();
    flag3 = 2;
  }
}

void checkForProjectile()
{
  if (flag4 == 1 && flag5 == 1) // if the projectile passed both speedometer sensors
  {
    velocity = (1000000 * (length) / (mark5 - mark4)); // speed calculation
    energy = 0.5 * mass * velocity * velocity * 1000;  // energy calculation
    duration = (mark5 - mark4);
    speed1 = (1000000 * (stage1distance) / (mark2 - mark1)); // duration in microseconds
    speed2 = (1000000 * (stage2distance) / (mark3 - mark2)); // duration in microseconds
    Serial.print("Shot ");
    Serial.println(n);              // output the shot number to CLI
    Serial.print("Stage1: ");       //
    Serial.print(speed1);           // output speed to command line
    Serial.println(" m/s");         //
    Serial.print("Stage2: ");       //
    Serial.print(speed2);           // output speed to command line
    Serial.println(" m/s");         //
    Serial.print("Stage3: ");       //
    Serial.print(velocity);         // output speed to command line
    Serial.println(" m/s");         //
    Serial.print("Energy: ");       //
    Serial.print(energy);           // output energy to command line
    Serial.println(" milliJoules"); //
    Serial.println("");
    Serial.print("Trigger 1 delay: ");
    Serial.println(mark2A - mark2);
    Serial.print("Trigger 2 delay: ");
    Serial.println(mark3A - mark3);
    delayMicroseconds(500000); // 0.5s delay timer
    flag1 = 0;                 //
    flag2 = 0;                 //
    flag3 = 0;                 //
    flag4 = 0;                 //
    flag5 = 0;                 //
    fireEnable = 0;            //
    n++;                       // shot number +1
  }
}

String newText;

void checkForFireCommand()
{
  if (Serial.available())
  {
    if (Serial.available() > 0)            // actually checks for any Serial transmission.
    {                                      // doesn't have to be "fire"
      newText = Serial.readString();
      Serial.println("FIRING COIL GUN");
      fireEnable = 1;                     // arms the coil gun
      digitalWrite(coil1, HIGH);          // fires the coil gun
      mark1 = micros();
    }
  }
}

void checkForFireButton()
{
  int fireButtonStatus = digitalRead(fireButton);
  if (fireButtonStatus == LOW)
  {
    Serial.println("FIRE BUTTON PRESSED");
    fireEnable = 1;                     // arms the coil gun
    digitalWrite(coil1, HIGH);          // fires the coil gun
    mark1 = micros();
  }
}

// Free to use this code under MIT open source license
// Copyright 2020 B.Sampson @kakushinAU

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files(the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and / or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions :

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS",
// WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.
