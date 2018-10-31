
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

char inData[70]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

void setup() {
Serial.begin(9600); // set the baud rate
Serial.println("Ready"); // print "Ready" once

  tft.reset();

//  uint16_t identifier = tft.readID();
//    Serial.print(F("LCD driver chip: "));
//    Serial.println(identifier, HEX);
    

//  tft.begin(identifier);

  // Serial.println(F("Benchmark                Time (microseconds)"));
 
  Serial.print(F("Text                     "));

  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
  tft.fillScreen(BLACK);
  


}
void loop() {

if(Serial.available()){ // only send data back if data has been sent
//char inByte = Serial.read(); // read the incoming data

            inChar = Serial.read(); // Read a character
            inData[index] = inChar; // Store it
            index++; // Increment where to write next
            inData[index] = '\0'; // Null terminate the string


Serial.println();

//  tft.fillScreen(BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(GREEN);
  tft.setTextSize(5);
  tft.drawString(inChar);
//  tft.println(inChar);
  
  delay(1000);
  
  tft.setCursor(0, 30);
  tft.setTextColor(BLACK);
  tft.setTextSize(5);
  tft.println(inChar);

Serial.println(inChar); // send the data back in a new line so that it is not all one long line
  }

//delay(100); // delay for 1/10 of a second
}
