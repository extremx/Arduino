#include <SoftwareSerial.h>;
 
//SoftwareSerial Serial1(10, 11); // RX, TX
int ledGRN = 2;
int ledBLU = 4;
int ledRED = 6;
int ledWHI = 8; 
int Serial1Data;
 
void setup() 
{
 Serial.begin(9600);
 Serial1.begin(9600);
 pinMode(ledGRN,OUTPUT);
 pinMode(ledBLU,OUTPUT);
 pinMode(ledRED,OUTPUT);
 pinMode(ledWHI,OUTPUT);
}
 
void loop()
{
 
 if (Serial1.available())
 {
 //  Serial.write(Serial1.read());
   Serial1Data=Serial1.read();
   Serial.println(Serial1Data); // This gives a weird output i.e.: an input of 2 will output 50 <newline> 10, but the  if(Serial1Data=='2’) still sees it properly. shrugs
//   digitalWrite(ledGRN, HIGH);
  
 if(Serial1Data=='1')
 { 
 digitalWrite(ledGRN, HIGH);
 Serial1.println("GREEN ON");
 }
  
 if(Serial1Data=='2')
 { 
 digitalWrite(ledBLU, HIGH);
 Serial1.println("BLUE ON");
 }
  
 if(Serial1Data=='3')
 { 
 digitalWrite(ledRED, HIGH);
 Serial1.println("RED ON");
 }
  
 if(Serial1Data=='4')
 { 
 digitalWrite(ledWHI, HIGH);
 Serial1.println("WHITE ON");
 }
  
 if (Serial1Data=='0')
 {
 digitalWrite(ledGRN,0);
 digitalWrite(ledBLU,0);
 digitalWrite(ledRED,0);
 digitalWrite(ledWHI,0);
 Serial1.println("LED'S OFF");
 }
  
 }
 delay(100);
}
