


// Use the display on the TS badge

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#include <SSD1306ASCII.h>


/*
 * Pin assignments (from schematic) updated to RC2.1
 * physical pin #, Arduino name, (alternate name / function), use on our board
 * 20 D0 (RX)  BLE HM-10
 * 21 D1 (TX)  BLE HM-10
 * 19 D2 (SDA) OLED display SSD1306
 * 18 D3~ (SCL)  OLED display SSD1306
 * 25 D4 (A6)  LPD_SDI data for LPD8806 RGB LED controller
 * 31 D5~    LPD_SCL clock for LPD8806 RGB LED controller
 * 27 D6~ (A7) Photoresistor input
 * 01 D7   IR input (output from IR receiver)
 * 28 D8 (A8)  IR high power (active low; this is the ground return through the resistor; set to INPUT to deactivate)
 * 29 D9~ (A9) IR low power (active low; this is the ground return through the resistor; set to INPUT to deactivate)
 * 30 D10~ (A10) Tilt switch (1 = hanging; 0 = flipped up)
 * 12 D11~     IR on back (active low; this is the ground return through the resistor; set to INPUT to deactivate)
 * 26 D12 (A12)  NC
 * 32 D13~ IR ouptut (goes to all LEDs; set ground pin [see above] as OUTPUT LOW to activate)
 * 11 D14 (MISO) SPI (ICSP & flash memory)
 * 09 D15 (SCK)  SPI (ICSP & flash memory)
 * 10 D16 (MOSI) SPI (ICSP & flash memory)
 * 08 D17 (SS) SPI chip select for flash memory (active low)
 * 36 A0 (D18) Battery voltage
 * 37 A1 (D19) Button 5 active low
 * 38 A2 (D20) Button 4 active low
 * 39 A3 (D21) Button 3 active low
 * 40 A4 (D22) Button 2 active low
 * 41 A5 (D23) Button 1 active low
 * ** D24 (D4/A6)  repeat
 * ** D25 (D6/A7)  repeat
 * ** D26 (D8/A8)  repeat
 * ** D27 (D9/A9)  repeat
 * ** D28 (D10/A10) repeat
 * ** D29 (D12/A11) repeat
 * 22 D30 (TXLED)  Photoresistor power
 */

#define countof(x) (sizeof(x) / sizeof((x)[0]))

SSD1306AsciiAvrI2c oled;

// the following two configurations are based on discussion at
// https://forums.adafruit.com/viewtopic.php?f=47&t=25438

// config when the connection pins are above display
static const uint8_t Adafruit128x64PinsAbove[] = {
    SSD1306_SEGREMAP | 0x1,            // column 127 mapped to SEG0
    SSD1306_COMSCANDEC                 // column scan direction reversed
};
// config when the connection pins are below display
static const uint8_t Adafruit128x64PinsBelow[] = {
    SSD1306_SEGREMAP,            // column 0 mapped to SEG0
    SSD1306_COMSCANINC           // column scan direction forward
};

//------------------------------------------------------------------------------
void setOrientation(byte dir)
{
  const uint8_t* table = dir == 0 ? Adafruit128x64PinsAbove : Adafruit128x64PinsBelow;
  for (uint8_t i = 0; i < sizeof(Adafruit128x64PinsAbove); i++) {
    oled.ssd1306WriteCmd(table[i]);
  }
}

// Tilt switch returns up/down status
const int TILT_SWITCH_PIN = 10;
#define TILT_UP 1
#define TILT_DOWN 0

//
// returns TILT_UP when the badge is facing the user direction (numbers right side up)
//
byte tiltSwitch(void)
{
  if (digitalRead(TILT_SWITCH_PIN) == 0)
    return TILT_UP;
  else
    return TILT_DOWN;
}


//------------------------------------------------------------------------------
void setup() 
{
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(System5x7);
  oled.set1X();
  oled.println("   //  \\\\");
  oled.println("  _\\\\()//_");
  oled.println(" / //  \\\\ \\ ");
  oled.println("  | \\__/ |");
  oled.println(" ");
  oled.set2X();
  oled.println("#LDSG");
  pinMode(TILT_SWITCH_PIN, INPUT_PULLUP); // tilt switch

}

#define TILT_DEBOUNCE_MILLIS 100

//------------------------------------------------------------------------------
void loop()
{

}

