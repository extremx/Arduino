// This sketch uses the relay sheild and a PIR sensor to detect motion
// and turn on a motor which raises the ghost, leaves it there for a time,
// then lowers it back down.

/*
 * PIR sensor tester
 */
int MotorControl = 6;    // J2
int MotorControlRev = 5;  // J3
int MotorControlFor = 4;  // J4 
int inputPin = 8;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
 
void setup() {
  pinMode(MotorControl, OUTPUT);
  pinMode(MotorControlRev, OUTPUT);
  pinMode(MotorControlFor, OUTPUT);
  
  pinMode(inputPin, INPUT);     // declare sensor as input
  digitalWrite(MotorControl,HIGH);// NO3 and COM3 Connected; the "ON" switch
  
  Serial.begin(9600);
  while (!Serial) {
; // wait for serial port to connect. Needed for Leonardo only
}
  Serial.println("Start Program");
}
 
void loop(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH


//MOTION! Turn motor on and crank up the ghost
    digitalWrite(MotorControlRev,HIGH);// NO3 and COM3 Connected;
    delay(10000); //How long are we cranking?

    digitalWrite(MotorControlRev,LOW);// NO3 and COM3 Connected
    delay(10000); //How long are we keeping it in the air?
    
    //Lets take it back down.
    digitalWrite(MotorControlFor,HIGH);// NO3 and COM3 Connected;
    delay(9000);

    //Stop the downward motor and waait for more motion
    digitalWrite(MotorControlFor,LOW);// NO3 and COM3 Connected
    delay(60000);  // 60 seconds

    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
//This is all for debugging and is not visible when in use
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}
