
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
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// SWTFT tft;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));



  tft.reset();

  uint16_t identifier = tft.readID();
    Serial.print(F("LCD driver chip: "));
    Serial.println(identifier, HEX);
    

  tft.begin(identifier);

  // Serial.println(F("Benchmark                Time (microseconds)"));
 
  Serial.print(F("Text                     "));
//  Serial.println(testText());

// }

// unsigned long testText() {
  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
 // tft.fillScreen(GREEN);
 // tft.fillScreen(BLUE);
  tft.fillScreen(BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(RED);    tft.setTextSize(3);
  tft.println(0xBAAAAAA, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextSize(5);
  tft.println("Arduino is awesomesaucebigword");
  tft.setTextSize(2);
  tft.println("IP: 208.129.36.5");
  tft.setTextSize(1);
  tft.println("Mac: AA:C0:D0:AC:B5:F1");
  tft.println("Uptime: 14:08:41 up 901 days,");
  tft.println("12457 MB Free");
//  return micros() - start;
}

void loop(void) {
  for(uint8_t rotation=0; rotation<4; rotation++) {
    tft.setRotation(rotation);
//    testText();
    delay(2000);
  }
}
