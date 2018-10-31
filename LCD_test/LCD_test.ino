/*
** Example Arduino sketch for SainSmart I2C LCD2004 adapter for HD44780 LCD screens
** Readily found on eBay or [...]
** The LCD2004 module appears to be identical to one marketed by YwRobot
**
** Edward Comer
** LICENSE: GNU General Public License, version 3 (GPL-3.0)
**
** sain_lcd_2.ino
** Simplified and modified by Andrew Scott for Arudino 1.0.1, Arudino Uno R3.
** NOTE: I2C pins are A4 SDA, A5 SCL
** Don't forget to use the new LiquidCrystal Library from [...]
*/

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
#include <dht.h>

dht DHT;
#define DHT11_PIN 7
#define I2C_ADDR 0x27 // Define I2C Address where the SainSmart LCD is
#define BACKLIGHT_PIN 3

int pin = 8;//DSM501A input D8
unsigned long duration;
unsigned long starttime;
unsigned long endtime;
unsigned long sampletime_ms = 15000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
int i=0;

//LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
LiquidCrystal_I2C lcd(I2C_ADDR,2,1,0,4,5,6,7);

void setup()
{
lcd.begin (20, 4);

//polution sensor init
Serial.begin(9600);
pinMode(8,INPUT);
starttime = millis();
  
// Switch on the backlight
lcd.setBacklightPin(3,POSITIVE);
lcd.setBacklight(HIGH);
lcd.home();
}

void loop() {
  
/*  duration = pulseIn(pin, LOW);
  lowpulseoccupancy += duration;
  endtime = millis();
  if ((endtime-starttime) > sampletime_ms)
  {
    
*/

  int chk = DHT.read11(DHT11_PIN);
//  Serial.println(DHT.temperature);
//  Serial.print("Humidity = ");
//  Serial.println(DHT.humidity);
//  delay(1000);

  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    int chk = DHT.read11(DHT11_PIN);
    int t = DHT.temperature;
    int h = DHT.humidity;
    
    lcd.setCursor ( 0, 0 );
    lcd.print("                     ");
    lcd.setCursor ( 0, 1 );
    lcd.print("                     ");
    lcd.setCursor ( 0, 2 );
    lcd.print("                     ");
    lcd.setCursor ( 0, 3 );
    lcd.print("                     ");
    
    lcd.setCursor ( 0, 0 );
    lcd.print("LowP: ");
    lcd.setCursor ( 6, 0 );
    lcd.print(lowpulseoccupancy);

    lcd.setCursor ( 0, 1 );
    lcd.print("Ratio: ");
    lcd.setCursor ( 7, 1 );
    lcd.print(ratio);

    lcd.setCursor ( 0, 2 );
    lcd.print("Concentr: ");
    lcd.setCursor ( 10, 2 );
    lcd.print(concentration);

    lcd.setCursor ( 0, 3 );
    lcd.print("temp:");
    lcd.setCursor ( 5, 3 );
//    lcd.print(DHT.temperature);
    lcd.print((int)round(1.8*t+32));
    
    lcd.setCursor ( 8, 3 );
    lcd.print("Humidity:");
    lcd.setCursor ( 17, 3 );
    lcd.print(h);   
    lcd.setCursor ( 19, 3 ); 
    lcd.print("%");

    
    lowpulseoccupancy = 0;
    starttime = millis();
  }
 
}

