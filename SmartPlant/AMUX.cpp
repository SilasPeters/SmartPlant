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
  digitalWrite(PIN_AMUX_SEL, LOW); // LOW or HIGH is inverted on NodeMCU
  readingMoist = true;
}

void AMUX::stopReadingMoist() {
  lastMoistReading = analogRead(PIN_AMUX_ANALOG_READ);
  digitalWrite(PIN_AMUX_SEL, HIGH); // LOW or HIGH is inverted on NodeMCU
  readingMoist = false;
}

void AMUX::readLdr() {
  lastLdrReading = analogRead(PIN_AMUX_ANALOG_READ);
}

#endif
