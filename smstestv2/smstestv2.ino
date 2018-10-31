#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8); // configure software serial port
// Note: Leonardo cannot have interrupts on all pins, so Rx can only be on 8,9,10,11,14,15,16
// Note from: http://arduino.cc/en/Reference/softwareSerial
 
void setup()
{
  pinMode(9, OUTPUT);
  Serial.begin(9600);
  SIM900.begin(19200);
  delay(2000);
  //SIM900power();  
  delay(60000);  // give time to log on to network.
  sendSMS();
}
 
void SIM900power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  Serial.println("Power Reset Start");
  delay(1000);
  digitalWrite(9, LOW);
  
  Serial.println("Power Reset End");
  delay(5000);
}
 
void callSomeone()
{
  SIM900.println("ATD + +12083518158;"); // dial UK phone
  Serial.println("ATD + +12083518158;");
  delay(100);
  SIM900.println();
  delay(10000);            // wait for 5 seconds...
  SIM900.println("ATH");   // hang up
  Serial.println("ATH - HANG UP");
}
 
void loop()
{  
  sendSMS();
  
  // callSomeone(); // call someone
  // SIM900power();   // power off GSM shield
  do {} while (1); // do nothing
}
 
void sendSMS()
{
  SIM900.print("AT+CMGF=1");
  // AT command to send SMS message
  delay(100);
  SIM900.println("AT + CMGS = \"+12083518158\""); 
  // recipient's mobile number, in international format
  delay(100);
  SIM900.println("Hello, world. This is a text message from an Arduino Uno.");  
  // message to send
  delay(100);
  SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(100);
  SIM900.println();
  delay(5000);       // give module time to send SMS
  //SIM900power();     // turn off module
}
