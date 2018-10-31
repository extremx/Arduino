// This program shown how to control arduino from PC Via Bluetooth
#include <Servo.h> 

//bluetooth crap
int yellowpin=4; // Yellow LED
int redpin=5; // RED LED
int purpin=6; // purple LED
int BluetoothData; // the data given from Computer
int btConst=0; // used to keep the last read value in the switch
int mode;

//servo crap
Servo servoLeft;            // Define left servo
Servo servoRight;           // Define right servo

//LineFollow crap
const int lineLSense = A2;
const int lineRSense = A3;
static int irReflectL = 0;
static int irReflectR = 0;
static int thresh = 650;

//Ultrasonic crap
const int pingPin = 11;   // Ping connected to D11
unsigned int duration, inches, cm;

Servo myServo;
const int delayTime = 2000;
const int servoPin = 8;


void setup() {
// bluetooth
  Serial.begin(9600); 
  delay(250);
  Serial.println("Bluetooth On: 0=stop, 1=Line, 2=ping, 3=LED's off, 4=controller");
  pinMode(yellowpin,OUTPUT);
  pinMode(redpin,OUTPUT);
  pinMode(purpin,OUTPUT);
//servos 
  myServo.attach(servoPin);
  servoLeft.attach(13); 
  servoRight.attach(12);
  servoLeft.write(95);
  servoRight.write(95);
  
}

#define STRAIGHT 0
#define LEFT 1
#define RIGHT 2

void loop() {
  /*  
  if (new BT data)
  {
    swith (bt data)
    {
      0: mode=stop;
      1: mode=line;
      2: mode=sonar;
      default: do nothing
    }
  }
    switch (mode)
    {
      case line:
        followline()
      
        stop();
    }
 */


  // put your main code here, to run repeatedly:
   if (Serial.available()) {

      BluetoothData=Serial.read();
      Serial.println("BT input data is: ");
//      Serial.println(BluetoothData); //chr ();  
      Serial.write(BluetoothData); 
      Serial.print("\n");   
   //  
    switch (BluetoothData) {
      case '0':
        mode=0;
        blinkStop();
        Serial.println("Stop Mode.");
      break;
      case '1':
        mode=1;
        blinkLine();
        Serial.println("Line Follow Mode");
      break;
      case '2':
        mode=2;
        blinkPing();        
        Serial.println("Ping Mode");
      break;
      case '3':
        alloff();
        mode=3;
      break;
      case '4':
        alloff();
        mode=4;
      break;         
      default: 
        //nothing
      break;    
    }
} 
//          Serial.println("Mode: ");
  //        Serial.println(mode);
    //      delay(750);
    switch (mode) {
      case 0:
          stopRobot();
        break;
      case 1:
          lineFollow();
        break;
      case 2:
          line_forward(); 
        if(sonarRead() < 6 && sonarRead() > 0) {       // Object ~5" or closer
          stopRobot();              // Stop, reverse, and turn
            delay(250);
          reverse();
            delay(500);
          line_spinRight();
            delay(750);
        }
        break;
      case 4:
       if (Serial.available()) {
        // read the oldest byte in the serial buffer:
        int incomingByte = Serial.read();
    
        // action depending on the instruction
        // as well as sending a confirmation back to the app
        switch (incomingByte) {
          case 'F':
            line_forward();
            Serial.println("Going forward");
            break;
          case 'R':
            line_slipRight();
            Serial.println("Turning right");
            break;
          case 'L':
            line_slipLeft();
            Serial.println("Turning left");
            break;
          case 'B':
            reverse();
            Serial.println("Going backwards");
            break;
          case 'S':
            stopRobot();
            Serial.println("Stopping");
            break;
        default: 
        // if nothing matches, do nothing
        break;
    }
  }
        break;      
      default: 
        //nothing
        break;
    }  
}

// Read Ping ultrasonic sensor
int sonarRead () {
  Serial.println("Reading Sonar Method");
  pinMode(pingPin, OUTPUT);          // Set pin to OUTPUT
  digitalWrite(pingPin, LOW);        // Ensure pin is low
  delayMicroseconds(2);  
  digitalWrite(pingPin, HIGH);       // Start ranging
  delayMicroseconds(5);              //   with 5 microsecond burst
  digitalWrite(pingPin, LOW);        // End ranging
  pinMode(pingPin, INPUT);           // Set pin to INPUT
  duration = pulseIn(pingPin, HIGH); // Read echo pulse
  inches = duration / 74 / 2;        // Convert to inches
  cm = duration / 29 / 2;            // Convert to centimeters
  return(inches);		     // Return inches (or cm)
  

}


void lineFollow() {
 
// Read reflective sensors
  irReflectL = analogRead(lineLSense);  
  irReflectR = analogRead(lineRSense);
  static int lastDir;

  if (irReflectL >= thresh && irReflectR >= thresh) {
    line_forward();   // on line
    lastDir = STRAIGHT; 
  }
  
  if (irReflectL >= thresh && irReflectR <= thresh) {
    line_spinLeft();  // veering off right
    delay(4);
    lastDir = LEFT;
  }
  
  if (irReflectL <= thresh && irReflectR >= thresh) {
    line_spinRight();  // veering off left
    delay(4);
    lastDir = RIGHT;
  }

  // If line is lost try to reacquire
  if (irReflectL < thresh && irReflectR < thresh) {
    if (lastDir == LEFT) 
      line_spinLeft();
    else 
      line_spinRight();
    delay(20);
  }
  
}


// Motion routines for line following
void line_forward() {
  servoLeft.write(180);
  servoRight.write(0);
//  Serial.println("line_forward");
}
void reverse() {  
  servoLeft.write(0);
  servoRight.write(180);
//  Serial.println("reverse");
}
void line_slipRight() {
  servoLeft.write(180);
  servoRight.write(90);
}
void line_slipLeft() {
  servoLeft.write(90);
  servoRight.write(0);
}
void line_spinRight() {
  servoLeft.write(180);
  servoRight.write(180);
//  Serial.println("line_spinRight");  
}
void line_spinLeft() {
  servoLeft.write(0);
  servoRight.write(0);
}
void stopRobot() {
  servoLeft.write(95);
  servoRight.write(95);
}

// MODE INDICATORS
void blinkStop() {
  digitalWrite(yellowpin,1); //stopped
}

void blinkLine() {
  digitalWrite(redpin,1); //Line follow
}

void blinkPing() {
  digitalWrite(purpin,1); //Ping mode
}

void alloff() {
  digitalWrite(purpin,0); //Ping mode
  digitalWrite(redpin,0); //Ping mode
  digitalWrite(yellowpin,0); //Ping mode
}
