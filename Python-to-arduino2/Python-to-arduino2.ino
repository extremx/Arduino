#include <Adafruit_GFX.h>    // Core graphics library
#include "SWTFT.h" // Hardware-specific library

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

SWTFT tft;

String txtMsg = "";                         // a string for incoming text
int lastStringLength = txtMsg.length();     // previous length of the String

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // send an intro:
  Serial.println("\n\nString  length():");
  Serial.println();
}

void loop() {
  // add any incoming characters to the String:
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    txtMsg += inChar;
  } 

  // print the message and a notice if it's changed:
  Serial.println(txtMsg);
  
  tft.setCursor(0, 30);
  tft.setTextColor(GREEN);
  tft.setTextSize(5);
  tft.println(inChar);
  
  delay(100);
  
  tft.setCursor(0, 30);
  tft.setTextColor(BLACK);
  tft.setTextSize(5);
  tft.println(inChar);
  
    // note the length for next time through the loop:
    lastStringLength = txtMsg.length();
  }
}
