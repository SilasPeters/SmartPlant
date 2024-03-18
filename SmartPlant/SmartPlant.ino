#include <Wire.h> // Provided by the Arduino core, not another library
// https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#i2c-wire-library
// https://github.com/esp8266/Arduino/blob/master/libraries/Wire/Wire.h

const int PIN_WIRE_DATA_LINE = D1;  // IC2 Data line
const int PING_WIRE_CLOCK_LINE = D2; // IC2 Clock line

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(PIN_WIRE_DATA_LINE, PING_WIRE_CLOCK_LINE);
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  oledSetup();
  bmpSetup();
  amuxSetup(5000, 160);
}

bool ledStatus = false;

void loop() {
  Serial.print(F("Temperature = "));
  Serial.print(readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(readAltitude()); // Adjusted to local forecast
  Serial.println(" m");

  Serial.println();
  
  oledLoop();
  /*
  amuxLoop();
  Serial.print(F("Last read LDR value = "));
  Serial.println(getLastLdrReading());

  Serial.print(F("Last read moist value = "));
  Serial.println(getLastMoistReading());

  Serial.println();
  Serial.println();

  ledStatus = !ledStatus;
  digitalWrite(LED_BUILTIN, ledStatus ? HIGH : LOW); */
  delay(1000);
}
