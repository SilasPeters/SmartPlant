#ifndef BMP_h
#define BMP_h

#include "BMP.h"

#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)


// Example from:
// https://github.com/adafruit/Adafruit_BMP280_Library/blob/master/examples/bmp280test/bmp280test.ino

void BMP::setup() {
  Serial.println(F("BMP280 test"));
  unsigned status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);

  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
    }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

float BMP::temperature() {
  return bmp.readTemperature();
}

float BMP::pressure() {
  return bmp.readPressure();
}

float BMP::altitude() {
  return bmp.readAltitude(1013.25); // Adjusted to local forecast
}

#endif