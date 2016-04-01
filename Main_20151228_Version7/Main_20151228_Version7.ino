/*
  Final Version

  Arduino Control the Servo Motor

  Main_20151221_Version7
  Created: 2015/11/20
  Modified: 2016/01/11
  by Dr. Pang & Tony & Keona

  Arduino: MEGA 2560
  Servo Motor: Panasonic AC Servo Motor Driver
*/

// Pin Assignment for Buttons
const int PowerPin = 13; // Power on the Servo Motor
const int MovePin = 12; // Shoot Button

const int CWPin = 11; // Rotate the Servo Motor CW
const int CCWPin = 10; // Rotate the Servo Motor CCW
const int Mode1Pin = 9; // Mode 1 (rotating angle, rod's starting position angle + speed) + name
const int Mode2Pin = 8; // Mode 2
const int Mode3Pin = 7; // Mode 3
const int Mode4Pin = 6; // Mode 4
const int Angle1Pin = 5; // Angle 1 __ degrees
const int Angle2Pin = 4; // Angle 2 __ degrees
const int Angle3Pin = 3; // Angle 3 __ degrees
const int SpeedPin = 2; // Adjust speed of rotating rod fast or slow


// Pin Assignment for Servo Motor (from Pin 34 to Pin 53)
const int VCCPin = 35;    // Pin: Power Supply +5V
const int SignPin = 37;   // Pin: Sign to Move
const int PulsePin = 39;  // Pin: Pulses
const int ServoOnPin = 41; // Pin: Turn the Servo Motor on (Step from HIGH to LOW)
const int DivPin = 43;    // Pin: Use the Electric Gear or not (Low with Gear)
const int CZPin = 45;     // Pin (INPUT):CZ
const int CoinPin = 47;   // Pin (INPUT):Control in Position

void setup() 
{
  // initialize the pins as a output:
  // Initialize the Control Buttons
  pinMode(PowerPin,INPUT);pinMode(MovePin,INPUT); 
  pinMode(CWPin,INPUT);pinMode(CCWPin,INPUT); // Rotating the servo motor clockwise or anti-clockwise
  pinMode(Mode1Pin,INPUT); pinMode(Mode2Pin, INPUT); pinMode(Mode3Pin, INPUT); pinMode(Mode4Pin, INPUT);
  pinMode(Angle1Pin, INPUT); pinMode(Angle2Pin, INPUT); pinMode(Angle3Pin, INPUT); // Adjust rotating angle
  pinMode(SpeedPin, INPUT); // Adjust speed of rotating rod fast or slow
  // Initialize the Pins connected to Servo Motor
  pinMode(34,OUTPUT);    pinMode(35,OUTPUT);    
  pinMode(36,OUTPUT);    pinMode(37,OUTPUT);
  pinMode(38,OUTPUT);    pinMode(39,OUTPUT);
  pinMode(40,OUTPUT);    pinMode(41,OUTPUT);
  pinMode(42,OUTPUT);    pinMode(43,OUTPUT);
  pinMode(44,OUTPUT);    pinMode(45,INPUT);
  pinMode(46,OUTPUT);    pinMode(47,INPUT);
  pinMode(48,OUTPUT);    pinMode(49,OUTPUT);
  pinMode(50,OUTPUT);    pinMode(51,OUTPUT);
  pinMode(52,OUTPUT);    pinMode(53,OUTPUT);
  
  // Set Pin's Default Voltages
  digitalWrite(ServoOnPin,HIGH);  // Servo Off
  digitalWrite(DivPin,HIGH);  // Electric Gear OFF
  digitalWrite(PulsePin,LOW);  
  digitalWrite(SignPin,HIGH);  // 
  digitalWrite(VCCPin,HIGH); // 5V 
  digitalWrite(34,LOW);    digitalWrite(36,LOW);
  digitalWrite(38,LOW);    digitalWrite(40,LOW);
  digitalWrite(42,LOW);    digitalWrite(44,LOW);
  digitalWrite(46,LOW);
}

int Power = LOW;  // Power on and off Button
int Move = LOW;   // Shoot Button

long PulseNum = 5000; // Set the Number of Pulses (Default)
                      // 90 degree: 5000 Pulses (Actual to servo is 50,000) 
long PulseNumDec = 0; // initialize deceleration pulse number (0.6 * PulseNum)
long PulseNumFinal= 0; //initialize remaining pulse numbers to make up to 20000
long Rangle = 0; // rotating rod's starting angle is zero degrees

// Button Control Settings
int PowerState = LOW;
int MoveState = LOW;
int MoveReadIn;
long t_time = 0;
long debounce = 200;
int RotateDeg = 0; // initialize the degree of rotation
int RotateSpeed = 0; // O means slow rotating speed, 1 means fast rotating speed
int MoveAngle = 0; // initialize move angle 
int MovePulseNum = 0; // initialize number of pulses to move

void loop()
{
  // Read the Power Button
  Power = digitalRead(PowerPin);  
  // Check for Power On the Motor
  if ((Power == HIGH)&&(PowerState == 0))
  {
     digitalWrite(ServoOnPin,LOW);  // Power the Servo Motor On
     PowerState = 1;
     delay(1);
  }

  // Check the Shoot Button
  MoveReadIn = digitalRead(MovePin);
  // One press, one cycle
  if (MoveReadIn == HIGH && millis() - t_time > debounce) 
  {
    if (MoveState == HIGH)
    {
      MoveState = LOW;
    }
    else
    {
      MoveState = HIGH;
    }
    t_time = millis();    
  }

  // Move the Servo Motor "Shoot" button ======================================================================
  if (MoveState == HIGH)
  {
      PulseNum = (5000/90)*RotateDeg;
      PulseNumDec = 0.6 * PulseNum; 
      PulseNumFinal = 20000 - PulseNum - PulseNumDec;
      if (PulseNum == 0)
      {
        PulseNumFinal = 0;
      }

      digitalWrite(SignPin,HIGH);  // Set the rotating direction of rod to counter-clockwise
      digitalWrite(SignPin,LOW);  // Set the rotating direction of rod to counter-clockwise
     
      if (RotateSpeed == 1)
      {
          Accelerate2(PulseNum); // Fast Speed
      } else {
          Accelerate(PulseNum); // Slow Speed                    
      }
      MoveState = LOW; 
   }

   // ======================================================================
   if (digitalRead(CWPin) == HIGH)
   {
      digitalWrite(SignPin,LOW);  
      digitalWrite(SignPin,HIGH); // Set the rotating direction of rod to clockwise
      for (long i=1; i<=(111); i++) {  //Sends 111 pulses
      digitalWrite(PulsePin,HIGH);
      delayMicroseconds(200); 
      digitalWrite(PulsePin,LOW);
      delayMicroseconds(200); 
       }
      Rangle = Rangle - 2; // decrease angle's counter by 2 degrees 
    }

   // ======================================================================
    if (digitalRead(CCWPin) == HIGH)
   {
      digitalWrite(SignPin,HIGH);  
      digitalWrite(SignPin,LOW);  // Set the rotating direction of rod to counter-clockwise
      for (long i=1; i<=(111); i++) {  //Sends 111 pulses
      delayMicroseconds(200); 
      digitalWrite(PulsePin,HIGH);
      delayMicroseconds(200); 
      digitalWrite(PulsePin,LOW);
      delayMicroseconds(200); 
      }
      Rangle = Rangle + 2; // increase angle's counter by 2 degrees 
    }

    // Mode 1:Serve Recieve ======================================================================
        // Rotating angle: 120
        // Speed: Fast
        // Starting angle: 75

    // Mode 1 Steps
        // Set rotating direction (SignPin HIGH or LOW) 
        // Set speed of rotation (RotateSpeed)
        // Position rod to starting position (MoveAngle, MovePulseNum)
     
   if (digitalRead(Mode1Pin) == HIGH) 
   {
      RotateDeg = 120; // rotating angle is 120 degrees
      RotateSpeed = 1; // fast rotating speed, accelerate2
      MoveAngle = 75 - Rangle; //calculate the angle to rotate for positioning the rod to start
      Rangle = 75; // new position of the rod
              
      PulseNum = (5000/90)*RotateDeg;
      PulseNumDec = 0.6 * PulseNum; 
      PulseNumFinal = 20000 - PulseNum - PulseNumDec;

      if (MoveAngle > 0)
      {
        digitalWrite(SignPin,HIGH);  
        digitalWrite(SignPin,LOW);  // Set the rotating direction of rod to counter-clockwise
        
         MovePulseNum = (5000/90)*MoveAngle; 
          for (long i=1; i<(MovePulseNum + 1); i++) {  //Sends MovePulseNum amount of pulses
                digitalWrite(PulsePin,HIGH); // set pin to 5V
                delayMicroseconds(200); 
                digitalWrite(PulsePin,LOW); // set pin to 0V (ground)
                delayMicroseconds(200); 
          }
      } else {
         digitalWrite(SignPin,LOW);  // Set the rotating direction of rod to clockwise
         digitalWrite(SignPin,HIGH);  
                
         MoveAngle = -1*MoveAngle; // Turns negative rotating degree to positive
         MovePulseNum = (5000/90)*MoveAngle; 
          for (long i=1; i<(MovePulseNum + 1); i++) {  //Sends MovePulseNum amount of pulses
                digitalWrite(PulsePin,HIGH); // set pin to 5V
                delayMicroseconds(200); 
                digitalWrite(PulsePin,LOW); // set pin to 0V (ground)
                delayMicroseconds(200); 
          }
      }
        

   }

     
    // Mode 2:Spike Recieve ======================================================================
        // Rotating angle: 120
        // Speed: Fast
        // Starting angle: 90

    // Mode 2 Steps
        // Set rotating direction (SignPin HIGH or LOW) 
        // Set speed of rotation (RotateSpeed)
        // Position rod to starting position (MoveAngle, MovePulseNum)
     
   if (digitalRead(Mode2Pin) == HIGH) 
   {
      RotateDeg = 120; // rotating angle is 120 degrees
      RotateSpeed = 1; // fast rotating speed, accelerate2
      MoveAngle = 90 - Rangle; //calculate the angle to rotate for positioning the rod to start
      Rangle = 90; // new position of the rod
              
      PulseNum = (5000/90)*RotateDeg;
      PulseNumDec = 0.6 * PulseNum; 
      PulseNumFinal = 20000 - PulseNum - PulseNumDec;
 

      if (MoveAngle > 0)
      {
       digitalWrite(SignPin,HIGH);  
       digitalWrite(SignPin,LOW);  // Set the rotating direction of rod to counter-clockwise
      
         MovePulseNum = (5000/90)*MoveAngle; 
          for (long i=1; i<(MovePulseNum + 1); i++) {  //Sends MovePulseNum amount of pulses
                digitalWrite(PulsePin,HIGH); // set pin to 5V
                delayMicroseconds(200); 
                digitalWrite(PulsePin,LOW); // set pin to 0V (ground)
                delayMicroseconds(200); 
          }
      } else {
         digitalWrite(SignPin,LOW);  // Set the rotating direction of rod to clockwise
         digitalWrite(SignPin,HIGH); 
          
         MoveAngle = -1*MoveAngle; // Turns negative rotating degree to positive
         MovePulseNum = (5000/90)*MoveAngle; 
          for (long i=1; i<(MovePulseNum + 1); i++) {  //Sends MovePulseNum amount of pulses
                digitalWrite(PulsePin,HIGH); // set pin to 5V
                delayMicroseconds(200); 
                digitalWrite(PulsePin,LOW); // set pin to 0V (ground)
                delayMicroseconds(200); 
          }
      }

       
   }

    // Mode 3:High set ======================================================================
        // Rotating angle: 70
        // Speed: Fast
        // Starting angle: 55

    // Mode 3 Steps
        // Set rotating direction (SignPin HIGH or LOW) 
        // Set speed of rotation (RotateSpeed)
        // Position rod to starting position (MoveAngle, MovePulseNum)
     
   if (digitalRead(Mode3Pin) == HIGH) 
   {
      RotateDeg = 70; // rotating angle is 70 degrees
      RotateSpeed = 1; // fast rotating speed, accelerate2
      MoveAngle = 55 - Rangle; //calculate the angle to rotate for positioning the rod to start
      Rangle = 55; // new position of the rod
               
      PulseNum = (5000/90)*RotateDeg;
      PulseNumDec = 0.6 * PulseNum; 
      PulseNumFinal = 20000 - PulseNum - PulseNumDec;

      if (MoveAngle > 0)
      {
       digitalWrite(SignPin,HIGH);  
       digitalWrite(SignPin,LOW);  // Set the rotating direction of rod to counter-clockwise
       
         MovePulseNum = (5000/90)*MoveAngle; 
          for (long i=1; i<(MovePulseNum + 1); i++) {  //Sends MovePulseNum amount of pulses
                digitalWrite(PulsePin,HIGH); // set pin to 5V
                delayMicroseconds(200); 
                digitalWrite(PulsePin,LOW); // set pin to 0V (ground)
                delayMicroseconds(200); 
          }
      } else {
        digitalWrite(SignPin,LOW);  // Set the rotating direction of rod to clockwise
        digitalWrite(SignPin,HIGH);
         
         MoveAngle = -1*MoveAngle; // Turns negative rotating degree to positive
         MovePulseNum = (5000/90)*MoveAngle; 
          for (long i=1; i<(MovePulseNum + 1); i++) {  //Sends MovePulseNum amount of pulses
                digitalWrite(PulsePin,HIGH); // set pin to 5V
                delayMicroseconds(200); 
                digitalWrite(PulsePin,LOW); // set pin to 0V (ground)
                delayMicroseconds(200); 
          }
      }

 
          
   }
   
    // Mode 4: Middle Set ======================================================================
        // Rotating angle: 45
        // Speed: Slow
        // Starting angle: 55

    // Mode 4 Steps
        // Set rotating direction (SignPin HIGH or LOW) 
        // Set speed of rotation (RotateSpeed)
        // Position rod to starting position (MoveAngle, MovePulseNum)
     
   if (digitalRead(Mode4Pin) == HIGH) 
   {
      RotateDeg = 45; // rotating angle is 45 degrees
      RotateSpeed = 0; // slow rotating speed, accelerate
      MoveAngle = 55 - Rangle; //calculate the angle to rotate for positioning the rod to start
      Rangle = 55; // new position of the rod
       
      PulseNum = (5000/90)*RotateDeg;
      PulseNumDec = 0.6 * PulseNum; 
      PulseNumFinal = 20000 - PulseNum - PulseNumDec;

     

      if (MoveAngle > 0)
      {
        digitalWrite(SignPin,HIGH);  
        digitalWrite(SignPin,LOW);  // Set the rotating direction of rod to counter-clockwise
         MovePulseNum = (5000/90)*MoveAngle; 
          for (long i=1; i<(MovePulseNum + 1); i++) {  //Sends MovePulseNum amount of pulses
                digitalWrite(PulsePin,HIGH); // set pin to 5V
                delayMicroseconds(200); 
                digitalWrite(PulsePin,LOW); // set pin to 0V (ground)
                delayMicroseconds(200); 
          }
      } else {
         digitalWrite(SignPin,LOW);  // Set the rotating direction of rod to clockwise
         digitalWrite(SignPin,HIGH);  
 
         MoveAngle = -1*MoveAngle; // Turns negative rotating degree to positive
         MovePulseNum = (5000/90)*MoveAngle; 
          for (long i=1; i<(MovePulseNum + 1); i++) {  //Sends MovePulseNum amount of pulses
                digitalWrite(PulsePin,HIGH); // set pin to 5V
                delayMicroseconds(200); 
                digitalWrite(PulsePin,LOW); // set pin to 0V (ground)
                delayMicroseconds(200); 
          }
      }

       
   }

// ======================================================================
    if (digitalRead(Angle1Pin) == HIGH)
   {
 
      RotateDeg = 50; 

        // Read in and set rotate speed:
        if (digitalRead(SpeedPin) == HIGH)
        {
           RotateSpeed = 1;  
        } else {
           RotateSpeed = 0;
        }
    }

// ======================================================================
    if (digitalRead(Angle2Pin) == HIGH)
   {
   
      RotateDeg = 70; 

      
        // Read in and set rotate speed:
        if (digitalRead(SpeedPin) == HIGH)
        {
           RotateSpeed = 1;  
        } else {
           RotateSpeed = 0;
        }
    }

// ======================================================================
    if (digitalRead(Angle3Pin) == HIGH)
   {

      RotateDeg = 90; 

      
        // Read in and set rotate speed:
        if (digitalRead(SpeedPin) == HIGH)
        {
           RotateSpeed = 1;  
        } else {
           RotateSpeed = 0;
        }
    }

// ======================================================================

  // Check for Power Off the Motor
  if ((Power == LOW)&&(PowerState == 1))
  {
     digitalWrite(ServoOnPin,HIGH);  // Power the Servo Motor Off
     PowerState = 0;
     delay(1);
  }
}//End: loop()

void Accelerate3(long PulseNum) {
   //Speed A:   delaymicrosecond(1) is used (pulse width around 300 us)
   for (long i=1; i<=100; i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(143); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(143); 
   }
}

void Accelerate2(long PulseNum){
  // Asseleration Period
  // Speed E: 5% delaymicrosecond(143 = 150-7) is used (pulse width around 300 us)
   for (long i=1; i<(PulseNum*0.05); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(143); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(143); 
   }
   // Speed D: 5% delaymicrosecond(48 = 55-7) is used (pulse width around 110 us)
   for (long i=1; i<(PulseNum*0.05); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(48); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(48); 
   }
   //Speed C: 10% delaymicrosecond(23=30-7) is used (pulse width around 60 us)
   for (long i=1; i<(PulseNum*0.1); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(23); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(23); 
   }
   //Speed B: 10% delaymicrosecond(14=21-7) is used (pulse width around 42 us)
   for (long i=1; i<(PulseNum*0.1); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(14); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(14); 
   }
   //Speed A: 10%  delaymicrosecond(7) is used (pulse width around 30 us)
   for (long i=1; i<=PulseNum*0.1; i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(7); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(7); 
   }

   //Speed AA: 60%  delaymicrosecond(1) is used (pulse width around 20 us)
   for (long i=1; i<=PulseNum*0.6; i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(1); // fastest speed
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(1); 
   }
   
   //Speed A: 15%  delaymicrosecond(7) is used (pulse width around 30 us)
   for (long i=1; i<=PulseNum*0.15; i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(7); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(7); 
   }
   
   //Speed B: 15% delaymicrosecond(14=21-7) is used (pulse width around 42 us)
   for (long i=1; i<(PulseNum*0.15); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(14); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(14); 
   }
   //Speed C: 10% delaymicrosecond(23=30-7) is used (pulse width around 60 us)
   for (long i=1; i<(PulseNum*0.10); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(23); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(23); 
   }
   // Speed D: 10% delaymicrosecond(48 = 55-7) is used (pulse width around 110 us)
   for (long i=1; i<(PulseNum*0.1); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(48); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(48); 
   }
   // Speed E: 10% delaymicrosecond(143 = 150-7) is used (pulse width around 300 us)
   for (long i=1; i<(PulseNum*0.1); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(143); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(143); 
   }

  // Speed E: Remainder pulses delaymicrosecond(143 = 150-7) is used (pulse width around 300 us)
   for (long i=1; i<(PulseNumFinal); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(143); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(143); 
   }
}

void Accelerate(long PulseNum){
  // Asseleration Period
  // Speed E: 5% delaymicrosecond(143 = 150-7) is used (pulse width around 300 us)
   for (long i=1; i<(PulseNum*0.05); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(143); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(143); 
   }
   // Speed D: 10% delaymicrosecond(48 = 55-7) is used (pulse width around 110 us)
   for (long i=1; i<(PulseNum*0.1); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(48); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(48); 
   }
   //Speed C: 15% delaymicrosecond(23=30-7) is used (pulse width around 60 us)
   for (long i=1; i<(PulseNum*0.15); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(23); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(23); 
   }
   //Speed B: 20% delaymicrosecond(14=21-7) is used (pulse width around 42 us)
   for (long i=1; i<(PulseNum*0.2); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(14); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(14); 
   }
   //Speed A: 50%  delaymicrosecond(7) is used (pulse width around 30 us)
   for (long i=1; i<=PulseNum*0.5; i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(7); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(7); 
   }
   //Speed B: 30% delaymicrosecond(14=21-7) is used (pulse width around 42 us)
   for (long i=1; i<(PulseNum*0.3); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(14); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(14); 
   }
   //Speed C: 15% delaymicrosecond(23=30-7) is used (pulse width around 60 us)
   for (long i=1; i<(PulseNum*0.15); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(23); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(23); 
   }
   // Speed D: 10% delaymicrosecond(48 = 55-7) is used (pulse width around 110 us)
   for (long i=1; i<(PulseNum*0.1); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(48); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(48); 
   }
   // Speed E: 5% delaymicrosecond(143 = 150-7) is used (pulse width around 300 us)
   for (long i=1; i<(PulseNum*0.05); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(143); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(143); 
   }
      // Speed E: Remainder pulses from 20000 delaymicrosecond(143 = 150-7) is used (pulse width around 300 us)
   for (long i=1; i<(PulseNumFinal); i++) {  
    digitalWrite(PulsePin,HIGH);
    delayMicroseconds(143); 
    digitalWrite(PulsePin,LOW);
    delayMicroseconds(143); 
   }
}

