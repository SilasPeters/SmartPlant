#ifndef AMUX_h
#define AMUX_h

#include "AMUX.h"

void AMUX::setup(int pin_sel, int pin_read, unsigned long moistReadingIntervalParam, unsigned long moistReadingDurationParam) {
  PIN_AMUX_SEL = pin_sel;
  PIN_AMUX_ANALOG_READ = pin_read;

  moistReadingInterval = moistReadingIntervalParam;
  moistReadingDuration = moistReadingDurationParam;

  pinMode(PIN_AMUX_SEL, OUTPUT);
  pinMode(PIN_AMUX_ANALOG_READ, INPUT);

  // Ensure that both values have been read before logic of other modules start
  readLdr();
  startReadingMoist();
  delay(moistReadingDuration); // TODO blocking
  stopReadingMoist();
}

int AMUX::getLastMoistReading() { return lastMoistReading; }
int AMUX::getLastLdrReading() { return lastLdrReading; }

void AMUX::loop() {
  return;
  unsigned long timeSinceLastMoistReading = millis() - lastMoistReadingTime;

  // Check if it is time to read the soil's moist
  if (timeSinceLastMoistReading >= moistReadingInterval) {
    startReadingMoist();
  }

  switch(readingMoist) { // Either measure the soil or the light, not both
    case true:
      // Stop reading the moist once enough time has passed
      if (timeSinceLastMoistReading >= moistReadingDuration) {
        stopReadingMoist();
      }
      break;

    case false:
      // Update LDR reading
      readLdr();
      break;
  }
}

void AMUX::startReadingMoist() {
  lastMoistReadingTime = millis();
  pinMode(PIN_AMUX_SEL, HIGH);
  readingMoist = true;
}

void AMUX::stopReadingMoist() {
  lastMoistReading = analogRead(PIN_AMUX_ANALOG_READ);
  pinMode(PIN_AMUX_SEL, LOW);
  readingMoist = false;
}

void AMUX::readLdr() {
  lastLdrReading = analogRead(PIN_AMUX_ANALOG_READ);
}

#endif

/* OLD MANUAL MODE
void startReadingMoist() {
  readingMoist = true;
  readingMoistTimeStart = millis();
  pinMode(PIN_AMUX_SEL, HIGH);
}

// Will automatically be called
// Is public to allow early stopping
void stopReadingMoist() {
  readingMoist = false;
  moistResult = analogRead(PIN_AMUX_ANALOG_READ);
  pinMode(PIN_AMUX_SEL, LOW);
}

// Only returns true when the measuring has stopped.
// If returning true, sets the reading to the measured moist reading.
bool tryUpdateMoistReading(*int reading) {
  if (readingMoist) { return false; }

  reading = moistResult;
  return true;
}
*/
