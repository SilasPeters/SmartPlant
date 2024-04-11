#ifndef OLED_h
#define OLED_h

#include "OLED.h"

#include <Wire.h> // Included in arduino core
#include <Adafruit_GFX.h> // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h> // https://github.com/adafruit/Adafruit_SSD1306

#define OLED_RESET -1
#define WIRE Wire

//Bitmap of froge
const unsigned char froge_bitmap_frogesmall [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x1f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 
	0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x38, 0x7f, 0xf8, 
	0x3f, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0xe0, 0x1f, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xf0, 0x00, 
	0x03, 0xc0, 0x05, 0xff, 0xff, 0xff, 0xff, 0xff, 0x88, 0x00, 0x0f, 0xcf, 0x0f, 0xfd, 0xef, 0xfb, 
	0xff, 0xff, 0xe0, 0x00, 0x19, 0xff, 0xff, 0x83, 0xff, 0xff, 0xff, 0xcf, 0x0f, 0x00, 0x30, 0x7f, 
	0xff, 0x30, 0x3f, 0xff, 0xff, 0x81, 0xcf, 0x00, 0x10, 0x00, 0xff, 0xf8, 0xcf, 0xff, 0xff, 0xe0, 
	0x21, 0xc0, 0x00, 0xc0, 0x0f, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3f, 0xff, 0xfc, 0x7f, 0xfe, 0x00, 0x30, 0x00, 0x80, 0x30, 0x71, 0xff, 0xff, 0xff, 0xfe, 0x00, 
	0x00, 0x1c, 0x30, 0xe0, 0x30, 0x4f, 0xff, 0x9f, 0xe7, 0x00, 0x00, 0x70, 0x43, 0xf0, 0x07, 0xc7, 
	0xff, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x87, 0xf0, 0x00, 0x00, 0xff, 0x3f, 0xf8, 0x00, 0x00, 0x00, 
	0x7f, 0xfe, 0x00, 0x00, 0xfb, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x01, 0xff, 
	0x80, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x37, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x04, 0x1f, 
	0xf8, 0x00, 0x3f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x07, 0x3f, 0xff, 0x00, 0x3f, 0xff, 0xf0, 0x00, 
	0x00, 0x00, 0x00, 0x1f, 0xff, 0xc3, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
	0xff, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x1f, 0xff, 0xf0, 0x01, 0xfc, 0x00, 0x00, 0x00, 
	0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 
	0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Seetup
void OLED::setup() {
  OLED::display = Adafruit_SSD1306(128, 32, &WIRE);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  // Clear the buffer.
  display.clearDisplay();

  // Set textsize to one row (8px) and set text color
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

// Access point for clearing OLED
void OLED::clear()
{
  display.clearDisplay();
}

//Access point for (re)setting cursor position (default 0,0)
void OLED::cursor(int x, int y)
{
  display.setCursor(x, y);
}

// Print all staged pixels to the screen
void OLED::finish()
{
  display.display();
}

// Froge
void OLED::drawFroge()
{
  clear();
  cursor();
  display.drawBitmap(0, 0, froge_bitmap_frogesmall, 74, 32, WHITE);
  finish();
}

// Add text to staged pixels
void OLED::drawText(int line, String text)
{
  cursor(0, line);
  display.print(text);
}

// Draw the entire sensor screen (all sensor values)
void OLED::sensorScreen(float temp, int light, int moist, float pressure)
{
  clear();

  //TEMP
  drawText(0, "Temp:  " + String(temp) + "'C");

  //LIGHT
  float lightF = (float)light / 1024;
  int lightOn = (int)(lightF * 10);
  String lightPrint = "Light: ";
  for(int l = 0; l < 10; l++)
  {
    if(l < lightOn) { lightPrint += "#"; }
    else { lightPrint += "-"; }
  }
  drawText(8, lightPrint);

  //MOIST
  float moistF = (float)moist / 1024;
  int moistOn = (int)(moistF * 10);
  String moistPrint = "Soil:  ";
  for(int m = 0; m < 10; m++)
  {
    if(m < moistOn) { moistPrint += "#"; }
    else { moistPrint += "-"; }
  }
  drawText(16, moistPrint);

  //Pressure:
  drawText(24, "Press: " + String(pressure) + "hPa");

  finish();
}

// Draw the entire LastWater Screen (displays time since last water and moisture level)
void OLED::lastWaterScreen(int minutesAgo, int moist)
{
  clear();
  drawText(0, F("Last Water:"));
  drawText(8, String(minutesAgo) + "m ago");

  //MOIST
  float moistF = (float)moist / 1024;
  int moistOn = (int)(moistF * 10);
  String moistPrint = "Soil:  ";
  for(int m = 0; m < 10; m++)
  {
    if(m < moistOn) { moistPrint += "#"; }
    else { moistPrint += "-"; }
  }
  drawText(24, moistPrint);
  finish();
}

// Display watering screen (displays during watering, shows indicative text and moisture level)
void OLED::wateringScreen(int moist)
{
  clear();
  drawText(0, F("Watering..."));

  //MOIST
  float moistF = (float)moist / 1024;
  int moistOn = (int)(moistF * 10);
  String moistPrint = "Soil:  ";
  for(int m = 0; m < 10; m++)
  {
    if(m < moistOn) { moistPrint += "#"; }
    else { moistPrint += "-"; }
  }
  drawText(16, moistPrint);
  finish();
}

#endif