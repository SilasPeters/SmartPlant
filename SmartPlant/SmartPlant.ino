#include <Wire.h> // Provided by the Arduino core, not another library
// https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#i2c-wire-library
// https://github.com/esp8266/Arduino/blob/master/libraries/Wire/Wire.h

const int WireDataLine = D1;  // IC2 Data line
const int WireClockLine = D2; // IC2 Clock line

void setup()
{
  Wire.begin(WireDataLine, WireClockLine);
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  bmpSetup();
}

void loop() {
  Serial.print(F("Temperature = "));
  Serial.print(readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(readAltitude(1013.25)); /* Adjusted to local forecast! */
  Serial.println(" m");

  Serial.println();
  delay(2000);
}
