#include <Wire.h> // Provided by the Arduino core, not another library
// https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#i2c-wire-library
// https://github.com/esp8266/Arduino/blob/master/libraries/Wire/Wire.h

#include "env.h"
#include "MainServo.h"
#include "OLED.h"
#include "BMP.h"
#include "AMUX.h"
#include "Wifi.h"
#include "MQTT.h"

// PIN CONFIG --------------------------------------------
const int PIN_WIRE_DATA_LINE = D1;  // IC2 Data line
const int PIN_WIRE_CLOCK_LINE = D2; // IC2 Clock line
const int PIN_SERVO = D8;
const int PIN_AMUX_SEL = D5;
const int PIN_AMUX_ANALOG_READ = A0;
// -------------------------------------------------------

// STATES ------------------------------------------------
bool automatic = true;
// -------------------------------------------------------

// EVENT READERS -----------------------------------------
bool doWater = false;
bool doDetermine = false;
bool doPublish = false;
// -------------------------------------------------------

int OLEDScreen = 0;

// EVENT INTERVALS ---------------------------------------
const unsigned long determineInterval = 300000;
unsigned long lastDetermine = 0;
const unsigned long publishInterval = 10000;
unsigned long lastPublish = 0;
unsigned long lastWaterUpdate = 0;
int lastWaterMinutesAgo = 0;
// -------------------------------------------------------

// TRESHOLDS ---------------------------------------------
int thMoist = 512;
// -------------------------------------------------------

MainServo servo;
OLED oled;
BMP bmp;
AMUX amux;
Wifi wifi;
WiFiClient wifiClient;
MQTT mqtt(wifiClient, &mqtt_waterPlant, &mqtt_publishValues, &mqtt_calibrateMoist);

void mqtt_waterPlant() {
  doWater = true;
}

void mqtt_publishValues() {
  doPublish = true;
}

void mqtt_calibrateMoist(int payload) {
  thMoist = payload;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(PIN_WIRE_DATA_LINE, PIN_WIRE_CLOCK_LINE);
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  oled.setup();
  oled.drawFroge();
  bmp.setup(); // MUST BE AFTER OLED
  amux.setup(PIN_AMUX_SEL, PIN_AMUX_ANALOG_READ, 5000, 160);
  servo.setup(PIN_SERVO);
  // Autoconnect to known AP, or become AP with specified credentials
  wifi.setup(AP_SSID, AP_PASSWORD); // Note, that the password must be at least 8 chars long
}

bool ledStatus = false;

void loop() {
  mqtt.loop();
  amux.loop();

  readEvents();

  updateEvents();
}

void toggleAutomatic()
{
  automatic = !automatic;
}

void updateEvents()
{
  unsigned long mil = millis();
  if(mil - lastPublish >= publishInterval)
  {
    doPublish = true;
    lastPublish = mil;
  }
  if(mil - lastDetermine >= determineInterval && automatic)
  {
    doDetermine = true;
    lastDetermine = mil;
  }
  if(mil - lastWaterUpdate >= 60000)
  {
    lastWaterMinutesAgo ++;
    lastWaterUpdate = mil;
  }
}

void readEvents()
{
  if(doWater)
  {
    doWater = false;
    lastWaterUpdate = millis();
    lastWaterMinutesAgo = 0;
    waterPlant();
  }
  if(doDetermine)
  {
    doDetermine = false;
    determineWater();
  }
  if(doPublish)
  {
    doPublish = false;
    publishValues();
  }
}

void waterPlant()
{
  //TODO: servo go brrr
}

void determineWater()
{
  if(amux.getLastMoistReading() < thMoist)
  {
    // Moist Cr1tikal!
    doWater = true;
  }
}

void publishValues()
{
  mqtt.publishMoist(amux.getLastMoistReading());
  mqtt.publishLight(amux.getLastLdrReading());
  mqtt.publishPressure(bmp.pressure());
  mqtt.publishTemperature(bmp.temperature());

  OLEDScreen ++;
  if(OLEDScreen == 3) { OLEDScreen = 0; }

  switch(OLEDScreen)
  {
    case 0:
      oled.sensorScreen(bmp.temperature(), amux.getLastLdrReading(), amux.getLastMoistReading(), bmp.pressure());
      break;
    case 1:
      oled.lastWaterScreen(lastWaterMinutesAgo);
      break;
    case 2:
      oled.drawFroge();
      break;
  }
}
