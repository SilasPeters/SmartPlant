const int PIN_AMUX_SEL = D5;
const int PIN_AMUX_ANALOG_READ = A0;

bool readingMoist = false;
unsigned long lastMoistReadingTime = 0;
unsigned long moistReadingInterval;
unsigned long moistReadingDuration;

word lastMoistReading;
word lastLdrReading;

void amuxSetup(unsigned long moistReadingIntervalParam, unsigned long moistReadingDurationParam) {
  moistReadingInterval = moistReadingIntervalParam;
  moistReadingDuration = moistReadingDurationParam;

  pinMode(PIN_AMUX_SEL, OUTPUT);
  pinMode(PIN_AMUX_ANALOG_READ, INPUT);

  // Ensure that both values have been read before logic of other modules start
  _readLdr();
  _startReadingMoist();
  delay(moistReadingDuration); // TODO blocking
  _stopReadingMoist();
}

int getLastMoistReading() { return lastMoistReading; }
int getLastLdrReading() { return lastLdrReading; }

void amuxLoop() {
  unsigned long timeSinceLastMoistReading = millis() - lastMoistReadingTime;

  // Check if it is time to read the soil's moist
  if (timeSinceLastMoistReading >= moistReadingInterval) {
    _startReadingMoist();
  }

  switch(readingMoist) { // Either measure the soil or the light, not both
    case true:
      // Stop reading the moist once enough time has passed
      if (timeSinceLastMoistReading >= moistReadingDuration) {
        _stopReadingMoist();
      }
      break;

    case false:
      // Update LDR reading
      _readLdr();
      break;
  }
}

void _startReadingMoist() {
  lastMoistReadingTime = millis();
  pinMode(PIN_AMUX_SEL, HIGH);
  readingMoist = true;
}

void _stopReadingMoist() {
  lastMoistReading = analogRead(PIN_AMUX_ANALOG_READ);
  pinMode(PIN_AMUX_SEL, LOW);
  readingMoist = false;
}

void _readLdr() {
  lastLdrReading = analogRead(PIN_AMUX_ANALOG_READ);
}

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
