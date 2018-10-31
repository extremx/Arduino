/*
GPRS Connect TCP

This sketch is used to test seeeduino GPRS_Shield's send SMS func.
to make it work, you should insert SIM card to Seeeduino GPRS 
and replace the phoneNumber,enjoy it!

create on 2013/12/5, version: 0.1
by lawliet.zou(lawliet.zou@gmail.com)
*/
#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Suli.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define I2C_ADDRESS 0x3C
#define PIN_TX    7
#define PIN_RX    8
#define BAUDRATE  9600
#define PHONE_NUMBER "2083518158"
#define MESSAGE  "hello,world"
SSD1306AsciiAvrI2c oled;


  
GPRS gprsTest(PIN_TX,PIN_RX,BAUDRATE);//RX,TX,BaudRate

void setup() {
  Serial.begin(9600);
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
  oled.setFont(Adafruit5x7);
  oled.clear();
  while(0 != gprsTest.init()) {
      delay(1000);
      Serial.print("init error\r\n");
      oled.println("init error\r\n");
  }
  oled.clear();  
  Serial.println("gprs init success");
  oled.println("gprs init success");
  Serial.println("start to send message ...");
  oled.println("start to send message ...");
  gprsTest.sendSMS(PHONE_NUMBER,MESSAGE); //define phone number and text
}

void loop() {
  //nothing to do
}
