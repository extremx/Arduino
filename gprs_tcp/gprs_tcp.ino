#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Suli.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define PIN_TX    7
#define PIN_RX    8
//make sure that the baud rate of SIM900 is 9600!
//you can use the AT Command(AT+IPR=9600) to set it through SerialDebug
#define BAUDRATE  9600

#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;

char http_cmd[] = "GET /media/uploads/mbed_official/hello.txt HTTP/1.0\r\n\r\n";
char buffer[512];
GPRS gprs(PIN_TX, PIN_RX, BAUDRATE);
void setup(){
  Serial.begin(9600);
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
  oled.setFont(Adafruit5x7);
  
  
  oled.clear();
  // use DHCP
  gprs.init();
  // attempt DHCP
  while(false == gprs.join(F("cmnet"))) {
      Serial.println("gprs join network error");
      oled.println("gprs join network error");
      delay(2000);
  }

  // successful DHCP
  oled.clear();
  Serial.print("IP Address is ");
  oled.println("IP Address is ");
  Serial.println(gprs.getIPAddress());
  oled.println(gprs.getIPAddress());

  if(false == gprs.connect(TCP,"mbed.org", 80)) {
      oled.clear();
      Serial.println("connect error");
      oled.println("connect error");
  }else{
      Serial.println("connect mbed.org success");
      oled.clear();
      oled.println("connect mbed.org success");      
  }
  oled.clear();
  Serial.println("waiting to fetch...");
  oled.println("waiting to fetch...");  
  gprs.send(http_cmd, sizeof(http_cmd)-1);
  while (true) {
      int ret = gprs.recv(buffer, sizeof(buffer)-1);
      if (ret <= 0){
          oled.clear();
          Serial.println("fetch over...");
          oled.println("fetch over...");
          break; 
      }
      buffer[ret] = '\0';
      oled.clear();
      Serial.print("Recv: ");
      oled.println("Recv: ");
      Serial.print(ret);
      oled.println(ret);
      Serial.print(" bytes: ");
      oled.println(" bytes: ");
      Serial.println(buffer);
      oled.println(buffer);
  }
  gprs.close();
  gprs.disconnect();
}

void loop(){

}
