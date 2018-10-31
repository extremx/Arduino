/* YourDuino Basic Robot Kit Collision-avoidance Test
 http://yourduino.com/sunshop2/index.php?l=product_detail&p=400
 - WHAT IT DOES:
 - Runs the robot motors
 - Looks Around with the Servo and Ultrasonic Sensor
 - SEE the comments after "//" on each line below
 NOTE: See the MoveAdjust values below to compensate for motor differences
       to make the robot run straighter.
 - CONNECTIONS:
 Pins 9 (Motor1 PWM) and 10 (Motor2 PWM) are predefined, unchangeable
 Motor1A  pin 2   // Motor pins to Driver board
 Motor1B  pin 4
 Motor2A  pin 7
 Motor2B  pin 8
 
 TRIGGER_PIN   5  // The Ultrasonic Sensor
 ECHO_PIN      6  
 
 SERVO_PIN    11  // The "Look Around" servo
 PIN13_LED    13  // Onboard LED. Lit when target in range
 
 - 
 - V1.20 01/28/2013  Smoother behavior
 Questions: terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <YD_MotorDriver1.h>  // For control of the two DC Motors
#include <NewPing.h>          // Runs the Ultrasonic Distance Sensor
#include <SM.h>;              // Implements the State Machine that controls action sequence

/*-----( Declare Constants and Pin Numbers )-----*/

// NOTE: Pins 9 (Motor1) and 10 (Motor2) are predefined, unchangeable
#define  Motor1A  2   // Motor pins to Driver board
#define  Motor1B  4
#define  Motor2A  7
#define  Motor2B  8
#define  RampDelay  10

#define  StartMoveSpeed    200  // Motor Driver value for start of motion

#define  VerySlowMoveSpeed     270
#define  VerySlowMoveAdjust     -4   // Adjust for straight move: - Left + Right??

#define  SlowMoveSpeed     280
#define  SlowMoveAdjust     -4   // Adjust for straight move: - Left + Right??

#define  MediumMoveSpeed   300
#define  MediumMoveAdjust  -8  // Adjust for straight move: - Left + Right

#define  FastMoveSpeed     350
#define  FastMoveAdjust    -8  // Adjust for straight move: - Left + Right

// Ultrasonic Sensor Pins
#define TRIGGER_PIN  5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
// Servo
#define SERVO_PIN    11  // The "Look Around" servo
#define PIN13_LED    13  // The onboard LED

//----( "TARGET FOUND" DIRECTIONS (index into TargetArray )---------
#define TARGET_FOUND_ANY     0   // Values will be "true" or "false"
#define TARGET_LEFT          1
#define TARGET_LEFT_CENTER   2
#define TARGET_CENTER        3
#define TARGET_RIGHT_CENTER  4
#define TARGET_RIGHT         5

#define TARGET_ARRAY_SIZE    6

#define TARGET_TOO_CLOSE     25

/*-----( Declare objects )-----*/
YD_MotorDriver1 RobotDriver(Motor1A,Motor1B,Motor2A,Motor2B); // Set pins
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Set pins and maximum distance.
SM RoboGo(RoboStartState);//create simple statemachine

/*-----( Declare Variables )-----*/
unsigned int uS;  // Result of a ping: MicroSeconds
unsigned int cm;  // Distance calculated for ping (0 = outside set distance range)
unsigned int cm_now; // For test

int TargetArray[TARGET_ARRAY_SIZE];  // Holds the directions a Target was found in
int DirectionsToLook = 3;  // For LookAround()
int ServoDirectionData[3] = { 2500, 1600, 600};



void setup()   /******************* SETUP: RUNS ONCE *****************/
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("YourDuino Robot Kit Test");
  //--NOTE: Ultrasonic Sensor and Motor Pins set to OUTPUT by their libraries
  pinMode(SERVO_PIN,OUTPUT);  
  pinMode(PIN13_LED,OUTPUT);    
  RobotDriver.init();

}//--(end setup )---

/************** LOOP: RUNS CONSTANTLY **************************/
void loop()   
{
  EXEC(RoboGo);//run statemachine
  delay(100);
}//--(end main loop )---


/*----------------( Declare User-written Functions )---------------*/

//------( MOTOR CONTROL FUNCTIONS )----------------
void ForwardVerySlow()
{
  RobotDriver.Motor1Speed(VerySlowMoveSpeed + VerySlowMoveAdjust);
  RobotDriver.Motor2Speed(VerySlowMoveSpeed - VerySlowMoveAdjust); 
}
/*---------------------------*/
void ForwardSlow()
{
  RobotDriver.Motor1Speed(SlowMoveSpeed + SlowMoveAdjust);
  RobotDriver.Motor2Speed(SlowMoveSpeed - SlowMoveAdjust); 
}
/*---------------------------*/

void ForwardMedium()
{
  RobotDriver.Motor1Speed(MediumMoveSpeed + MediumMoveAdjust);
  RobotDriver.Motor2Speed(MediumMoveSpeed - MediumMoveAdjust); 
}
/*---------------------------*/
void ForwardFast()
{
  RobotDriver.Motor1Speed(FastMoveSpeed + FastMoveAdjust);
  RobotDriver.Motor2Speed(FastMoveSpeed - FastMoveAdjust); 
}
/*---------------------------*/
void BackwardSlow(int HowMuch)
{
  RobotDriver.Motor1Speed(- SlowMoveSpeed );
  RobotDriver.Motor2Speed(- SlowMoveSpeed ); 
  delay(HowMuch);
  Stop();  
}
/*---------------------------*/
void BackwardMedium(int HowMuch)
{
  RobotDriver.Motor1Speed(- MediumMoveSpeed);
  RobotDriver.Motor2Speed(- MediumMoveSpeed); 
  delay(HowMuch);
  Stop(); 
}
/*---------------------------*/
void BackwardFast(int HowMuch)
{
  RobotDriver.Motor1Speed(- FastMoveSpeed);
  RobotDriver.Motor2Speed(- FastMoveSpeed); 
  delay(HowMuch);
  Stop(); 
}
/*---------------------------*/
void Stop()
{
  RobotDriver.Motor1Speed(0);
  RobotDriver.Motor2Speed(0); 
}
/*---------------------------*/
void SpinLeft(int HowMuch)
{
  RobotDriver.Motor1Speed(  MediumMoveSpeed);
  RobotDriver.Motor2Speed(- MediumMoveSpeed);
  delay(HowMuch);
  Stop();
}
/*---------------------------*/
void SpinRight(int HowMuch)
{
  RobotDriver.Motor1Speed(- MediumMoveSpeed);
  RobotDriver.Motor2Speed(  MediumMoveSpeed);
  delay(HowMuch);
  Stop(); 
}
/*---------------------------*/
unsigned int PingBlink()
{
  uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  cm = uS / US_ROUNDTRIP_CM; // Convert ping time to distance in cm 
  Serial.print("  cm = ");   
  Serial.print(cm,DEC);  

  if ((cm < 40) && (cm != 0))
  {
    return(cm);
  }
  else
  {
    return(100);  // No Valid Distance
  }
}// end PingBlink
/*---------------------------*/

void PointServo(int ServoAngle)
{

  for (int i=0;i<20;i++)  // Send the pulse 10 times 
  {
    digitalWrite(SERVO_PIN,HIGH);
    delayMicroseconds(ServoAngle);
    digitalWrite(SERVO_PIN,LOW);
    delay(20);
  }
}// PointServo end
/*---------------------------*/


void LookAround()  // Sets next state if Target Found
{
  for(int Direction = 0; Direction < DirectionsToLook ; Direction ++)
  {
    Serial.print("DIRECTION = ");     
    Serial.print(Direction,DEC);
    PointServo(ServoDirectionData[Direction]); // Get servo pulse width from array
    delay(200);
    cm_now = PingBlink();    // Read the Ultrasonic distance

    Serial.print(" cm_now = ");     
    Serial.println(cm_now,DEC);    
    if (cm_now < TARGET_TOO_CLOSE) digitalWrite(PIN13_LED,HIGH);    

    if (cm_now < 40)
    {
      TargetArray[TARGET_FOUND_ANY ] = true;
    }
    else TargetArray[TARGET_FOUND_ANY ] = false;

    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 0)) //LEFT
    {
      TargetArray[TARGET_LEFT ] = true;
      Serial.println("TargetLeft");  
    }    

    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 1)) //Center
    {
      TargetArray[TARGET_CENTER ] = true;
      Serial.println("TargetCenter");       
    }    

    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 2)) //RIGHT
    {
      TargetArray[TARGET_RIGHT ] = true;
      Serial.println("TargetRight");       
    }    

  }// END Directions

}// END LookAround

/**************************( STATE MACHINE FUNCTIONS )******************************/
State RoboStartState()
{
  Serial.println("+++ RoboStartState");
  digitalWrite(PIN13_LED,LOW);    // LED Means Target Too Close
  ForwardFast();  // Start moving forward
  delay(25);
  ForwardSlow();  // Start moving forward
  delay(600);
  RoboGo.Set(RoboStopLookState);    

}// END State RoboStartState


State RoboStopLookState()
{
  Serial.println("+++ RoboStopLookState");

  for (int i = 0; i < TARGET_ARRAY_SIZE; i++) TargetArray[i] = false;
  ForwardVerySlow();
  LookAround();     // Ping Ultrasonic in different directions, Set TargetArray

  if      (TargetArray[TARGET_CENTER ] == true)  RoboGo.Set(RoboTargetCenter);
  else if (TargetArray[TARGET_LEFT ]   == true)  RoboGo.Set(RoboTargetLeft);
  else if (TargetArray[TARGET_RIGHT ]  == true)  RoboGo.Set(RoboTargetRight);
  else     RoboGo.Set(RoboStartState);  

}// END State RoboStartState


State RoboTargetLeft()  // Something on the left, so Turn Right
{
  Serial.println("***** RoboTargetLeft");
  Stop();
  BackwardSlow(500);
  SpinRight(500);
  RoboGo.Set(RoboStartState);  
}// END State RoboTargetLeft ----------------------

State RoboTargetCenter() // Something ahead, so Turn Around
{
  Serial.println("***** RoboTargetCenter");  
  Stop();
  BackwardSlow(500);  
  SpinLeft(1400);
  RoboGo.Set(RoboStartState); 
} // END State RoboTargetCenter -----------------

State RoboTargetRight() // Something on the right, so Turn Left
{
  Serial.println("***** RoboTargetRight");   
  Stop();
  delay(500);
  BackwardSlow(500);
  SpinLeft(500);  
  RoboGo.Set(RoboStartState); 
}// END State RoboTargetRight -----------------

//*********( THE END )***********
