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
#include "GlobalStates.h"

// PIN CONFIG --------------------------------------------
const int PIN_WIRE_DATA_LINE = D1;  // IC2 Data line
const int PIN_WIRE_CLOCK_LINE = D2; // IC2 Clock line
const int PIN_SERVO = D8;
const int PIN_AMUX_SEL = D5;
const int PIN_AMUX_ANALOG_READ = A0;
// -------------------------------------------------------


// STATES ------------------------------------------------
bool AUTOMATIC = true;
// -------------------------------------------------------

// BUTTON VARS -------------------------------------------
unsigned long lastPress;
const unsigned long debounceTime = 50;
// -------------------------------------------------------

// WATER GIVING VARS -------------------------------------
const int waterOpen = 30;
const int waterClosed = 0;
bool isWatering = false;
unsigned long lastOpen;
const unsigned long openDelay = 10000;
int defaultSkipChecks = 2;
int skipChecks = defaultSkipChecks;
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
MQTT mqtt(wifiClient, &mqtt_waterPlant, &mqtt_publishValues, &mqtt_calibrateMoist, &setManual);

// Methods passed down to mqtt
void mqtt_waterPlant()                { doWater = true; }
void mqtt_publishValues()             { doPublish = true; }
void mqtt_calibrateMoist(int payload) { thMoist = payload; }

void setup()
{
  //setup flash button
  pinMode(0, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(PIN_WIRE_DATA_LINE, PIN_WIRE_CLOCK_LINE);
  Serial.begin(9600);
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
  readFlashButton();

  mqtt.loop();
  amux.loop();

  readEvents();

  updateEvents();
}

//polls the flashbutton including debouncing, and toggles automatic if so
void readFlashButton()
{
  if(millis() - lastPress >= debounceTime)
  {
    if(digitalRead(0) == LOW)
    {
      toggleAutomatic();
      Serial.println("Toggled automatic");
    }
  }
}

//sets the AUTOMATIC variable (updated from MQTT input)
void setManual(bool manual)
{
  AUTOMATIC = !manual;
  setAutomaticLed();
}

//flips the AUTOMATIC variable (from button, updates MQTT output)
void toggleAutomatic()
{
  AUTOMATIC = !AUTOMATIC;
  mqtt.publishManual(!AUTOMATIC);
  setAutomaticLed();  
}

//Updates builtin led
void setAutomaticLed()
{
  if(AUTOMATIC)
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  else 
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }  
}

//Checks for all events that have to occur.
void updateEvents()
{
  checkIfCloseWater();

  unsigned long mil = millis();
  if(mil - lastPublish >= publishInterval)
  {
    doPublish = true;
    lastPublish = mil;
  }
  if(mil - lastDetermine >= determineInterval && AUTOMATIC)
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

// Checks for the execution of any events and executes them if needed
void readEvents()
{
  if(doWater)
  {
    doWater = false;
    lastWaterUpdate = millis();
    lastWaterMinutesAgo = 0;
    waterPlant();
    mqtt.publishTimeSinceLastWatering(lastWaterMinutesAgo);
  }
  if(doDetermine)
  {
    doDetermine = false;
    if(skipChecks == 0) // Skip first two checks after a water
    {
      determineWater();
    }
    else
    {
      skipChecks --;
    }
  }
  if(doPublish)
  {
    doPublish = false;
    publishValues(); // Updates MQTT values
  }
}

//Check if the watering should stop, and if so update the OLED back to its original cycle and close the watering.
void checkIfCloseWater()
{
  if(isWatering && millis() - lastOpen > openDelay)
  {
    isWatering = false;
    servo.write(waterClosed);
    updateOLED();
  }
}

// Start the watering of the plant.
void waterPlant()
{
  isWatering = true;
  lastOpen = millis();
  servo.write(waterOpen);
  skipChecks = defaultSkipChecks;
  oled.wateringScreen(amux.getLastMoistReading());
}

// Determine if watering should occur
void determineWater()
{
  if(amux.getLastMoistReading() < thMoist)
  {
    // Moist Cr1tikal!
    doWater = true;
  }
}

// Publish values to MQTT
void publishValues()
{
  mqtt.publishMoist(amux.getLastMoistReading());
  mqtt.publishLight(amux.getLastLdrReading());
  mqtt.publishPressure(bmp.pressure());
  mqtt.publishTemperature(bmp.temperature());
  mqtt.publishTimeSinceLastWatering(lastWaterMinutesAgo);

  if(!isWatering)
  {
    updateOLED(); // Update screen only if it's not watering
  }
}

// Update the ODED screen to cycle between various screens.
void updateOLED()
{
  OLEDScreen ++;
  if(OLEDScreen == 3) { OLEDScreen = 0; }

  switch(OLEDScreen)
  {
    case 0:
      oled.sensorScreen(bmp.temperature(), amux.getLastLdrReading(), amux.getLastMoistReading(), bmp.pressure());
      break;
    case 1:
      oled.lastWaterScreen(lastWaterMinutesAgo, amux.getLastMoistReading());
      break;
    case 2:
      oled.drawFroge();
      break;
  }
}
