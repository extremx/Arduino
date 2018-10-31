// This program shown how to control arduino from PC Via Bluetooth
// D11   >>>  Rx
// D10   >>>  Tx
#include <SoftwareSerial.h>// import the serial library
#include <Servo.h> 

//bluetooth crap
SoftwareSerial blueTooth(5, 6); // RX, TX
int ledpin=4; // led on D13 will show blink on / off
int BluetoothData; // the data given from Computer
int btConst=0; // used to keep the last read value in the switch

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
  blueTooth.begin(9600);
  blueTooth.println("Bluetooth On: 0=stop, 1=Line, 2=ping");
  pinMode(ledpin,OUTPUT);
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
  // put your main code here, to run repeatedly:
   if (blueTooth.available()){
      BluetoothData=blueTooth.read();
//      if (BluetoothData=blueTooth.read() == 0 || BluetoothData=blueTooth.read() == 1 || BluetoothData=blueTooth.read() == 2 ||  BluetoothData=blueTooth.read() == 3 ) {
      if (BluetoothData == 0 || BluetoothData == 1 || BluetoothData == 2 ||  BluetoothData == 3 ) {  
          btConst = BluetoothData;
     }
      
      blueTooth.println("BT input data is: ");
      blueTooth.println(BluetoothData);  
      
    switch (btConst) {
      case '0':
          blinkStop();
          blueTooth.println("Stop Mode.");
          stopRobot();
        break;
      case '1':
          blinkLine();
          blueTooth.println("Line Follow Mode");
          lineFollow();
        break;
      case '2':
          blinkPing();
          blueTooth.println("Ping Mode");
          line_forward(); 
          blueTooth.println("Forward, HO!");
        if(sonarRead() < 6 && sonarRead() > 0) {       // Object ~5" or closer
          stopRobot();              // Stop, reverse, and turn
            delay(250);
          reverse();
            delay(500);
          line_spinRight();
            delay(750);
        }
        break;
      case '3':

        break;
      default: 
        // if nothing matches, do nothing
        break;
    }
   }
}

/*       
  if (BluetoothData=='0'){// if number 0 pressed .... Reset to "stop"
    blinkStop();
    blueTooth.println("Stop Mode.");
    stopRobot();
         }
  
   if(BluetoothData=='1'){   // if number 1 pressed .... Follow a line.
     blinkLine();
     blueTooth.println("Line Follow Mode");
     lineFollow();
         }
   if(BluetoothData=='2'){   // if number 2 pressed .... Ping mode.
     blinkPing();
     blueTooth.println("Ping Mode");
       line_forward(); 
       blueTooth.println("Forward, HO!");
      if(sonarRead() < 6) {       // Object ~5" or closer
        stopRobot();              // Stop, reverse, and turn
          delay(250);
        reverse();
          delay(500);
        line_spinRight();
          delay(750);
        }
    }


//delay(100);// prepare for next data ...
  }
}
*/
// Read Ping ultrasonic sensor
int sonarRead () {
  blueTooth.println("Reading Sonar Method");
  pinMode(pingPin, OUTPUT);          // Set pin to OUTPUT
  blueTooth.println("set pin to output");
  digitalWrite(pingPin, LOW);        // Ensure pin is low
  blueTooth.println("set pin low");
  delayMicroseconds(2);  
  digitalWrite(pingPin, HIGH);       // Start ranging
  blueTooth.println("start ranging");  
  delayMicroseconds(5);              //   with 5 microsecond burst
  digitalWrite(pingPin, LOW);        // End ranging
  blueTooth.println("End ranging");
  pinMode(pingPin, INPUT);           // Set pin to INPUT
  blueTooth.println("set pin to input");
  duration = pulseIn(pingPin, HIGH); // Read echo pulse
  blueTooth.println("read echo pulse");  
  inches = duration / 74 / 2;        // Convert to inches
  cm = duration / 29 / 2;            // Convert to centimeters
  blueTooth.println("Done with Sonar Method");
  blueTooth.println(inches);
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
  blueTooth.println("line_forward");
}
void reverse() {  
  servoLeft.write(0);
  servoRight.write(180);
  blueTooth.println("reverse");
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
  blueTooth.println("line_spinRight");  
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
  digitalWrite(ledpin,1); //stopped
    delay(100);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
    delay(100);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
    delay(100);  
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
    delay(100);  
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
    delay(100);    
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
  
}

void blinkLine() {
  digitalWrite(ledpin,1); //Line follow
    delay(300);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
    delay(300);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
      delay(100);
  digitalWrite(ledpin,1);
    delay(300);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
      delay(100);
  digitalWrite(ledpin,1);
    delay(300);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
}

void blinkPing() {
  digitalWrite(ledpin,1); //Ping mode
    delay(1000);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
    delay(1000);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
      delay(1000);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
    delay(1000);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
      delay(1000);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
    delay(1000);
  digitalWrite(ledpin,0);
    delay(100);
  digitalWrite(ledpin,1);
}
