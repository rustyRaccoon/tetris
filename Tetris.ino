// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
//Technical support:goodtft@163.com

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>

// The control pins for the LCD can be assigned to any digital or analog pins...but we'll use the analog pins as this allows us to double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

//Define the pins for touchscreen
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

#define TS_minX 0
#define TS_maxX 1023
#define TS_minY 0
#define TS_maxY 1023
#define minPressure 10
#define maxPressure 1000

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// If using the shield, all control and data lines are fixed, and a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

  tft.reset();
  tft.invertDisplay(1);

  uint16_t identifier = tft.readID();
  //  if (identifier == 0x9325) {
  //    Serial.println(F("Found ILI9325 LCD driver"));
  //  }
  //  else if (identifier == 0x9328) {
  //    Serial.println(F("Found ILI9328 LCD driver"));
  //  }
  //  else if (identifier == 0x4535) {
  //    Serial.println(F("Found LGDP4535 LCD driver"));
  //  }
  //  else if (identifier == 0x7575) {
  //    Serial.println(F("Found HX8347G LCD driver"));
  //  }
  //  else if (identifier == 0x9341) {
  //    Serial.println(F("Found ILI9341 LCD driver"));
  //  }
  //  else if (identifier == 0x7783) {
  //    Serial.println(F("Found ST7781 LCD driver"));
  //  }
  //  else if (identifier == 0x8230) {
  //    Serial.println(F("Found UC8230 LCD driver"));
  //  }
  //  else if (identifier == 0x8357) {
  //    Serial.println(F("Found HX8357D LCD driver"));
  //  }
  //  else if (identifier == 0x0101)
  //  {
  //    identifier = 0x9341;
  //    Serial.println(F("Found 0x9341 LCD driver"));
  //  }
  //  else if (identifier == 0x9481) {
  //    Serial.println(F("Found 0x9481 LCD driver"));
  //  }
  //  else if (identifier == 0x9486) {
  //    Serial.println(F("Found 0x9486 LCD driver"));
  //  }
  //  else
  //  {
  //    Serial.print(F("Unknown LCD driver chip: "));
  //    Serial.println(identifier, HEX);
  //    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
  //    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
  //    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
  //    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
  //    Serial.println(F("Also if using the breakout, double-check that all wiring"));
  //    Serial.println(F("matches the tutorial."));
  //    identifier = 0x9486;
  //  }

  tft.begin(identifier);
  tft.setRotation(0);
  printWelcome();
}

void loop(void)
{
  tft.fillScreen(WHITE);
  unsigned long start = micros();
  tft.drawRect(0, 0, 20, 20, BLACK); //xPos,yPos,xSize,ySize,color
  delay(1000);
}

void printWelcome(void)
{
  tft.setTextColor(BLACK);
  tft.setTextSize(6);
  tft.setCursor(35, 110); //xPos,yPos
  tft.println("WELCOME");
  tft.setCursor(30, 180);
  tft.println("MR USER");
  tft.println();
  tft.setTextSize(2);
  tft.println("I can see you have once");
  tft.println("again returned to play");
  tft.println("another game.");
  tft.println();
  tft.println("When you're ready, do the thing...");

  tft.drawRect((tft.width() / 2) - 40, tft.height() - 80, 80, 40, BLUE);
  tft.setCursor((tft.width() / 2) - 30, tft.height() - 66); //80-14 = 66
  tft.println("Start");

  bool userPressedStart = false;
  bool touchArmed = true;

  Serial.println((tft.width() / 2) - 40);
  Serial.println((tft.width() / 2) + 40);
  Serial.println();
  Serial.println(tft.height() - 80);
  Serial.println(tft.height() - 40);

  while (!userPressedStart)
  {
    TSPoint touchPoint = ts.getPoint();

    touchPoint.x = touchPoint.x + touchPoint.y;
    touchPoint.y = touchPoint.x - touchPoint.y;
    touchPoint.x = touchPoint.x - touchPoint.y;

    //DOESNT FULLY WORK. DUNNO WHY
    //map point to tft (0->1023 to tft.width)
    touchPoint.x = map(touchPoint.x, TS_minX, TS_maxX, tft.width(), 0);
    touchPoint.y = tft.height() - map(touchPoint.y, TS_minY, TS_maxY, tft.height(), 0);

    if (touchPoint.z > minPressure && touchPoint.z < maxPressure && touchArmed == true)
    {
      touchArmed = false;
      Serial.print(F("Touchpoint: x = ")); Serial.print(touchPoint.x); Serial.print(F(" | y = ")); Serial.print(touchPoint.y); Serial.print(F(" | z = ")); Serial.print(touchPoint.z);
      if (touchPoint.x > ((tft.width() / 2) - 40) && touchPoint.x < ((tft.width() / 2) + 40))
      {
        Serial.println(F("Got into x loop"));
        if (touchPoint.y > (tft.height() - 80) && touchPoint.y < (tft.height() - 40))
        {
          Serial.println(F("Got into y loop"));
          userPressedStart = true;
        }
      }
      touchArmed = true;
      Serial.println();
    }
  }

  loop();
}
