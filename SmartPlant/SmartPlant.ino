#include <Wire.h> // Provided by the Arduino core, not another library
// https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#i2c-wire-library
// https://github.com/esp8266/Arduino/blob/master/libraries/Wire/Wire.h

#include "MainServo.h"
#include "OLED.h"
#include "BMP.h"
#include "AMUX.h"
#include "Wifi.h"

//PIN CONFIG --------------------------------------------
const int PIN_WIRE_DATA_LINE = D1;  // IC2 Data line
const int PIN_WIRE_CLOCK_LINE = D2; // IC2 Clock line
const int PIN_SERVO = D8;
const int PIN_AMUX_SEL = D5;
const int PIN_AMUX_ANALOG_READ = A0;
//-------------------------------------------------------

MainServo servo;
OLED oled;
BMP bmp;
AMUX amux;
Wifi wifi;

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
  wifi.setup("All hail the Froge", "Aap noot mies"); // Note, that the password must be at least 8 chars long
}

bool ledStatus = false;

void loop() {

  Serial.print(F("Temperature = "));
  Serial.print(bmp.temperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.pressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.altitude()); // Adjusted to local forecast
  Serial.println(" m");

  Serial.println();
  
  amux.loop();
  Serial.print(F("Last read LDR value = "));
  Serial.println(amux.getLastLdrReading());

  Serial.print(F("Last read moist value = "));
  Serial.println(amux.getLastMoistReading());

  Serial.println();
  Serial.println();

  ledStatus = !ledStatus;
  digitalWrite(LED_BUILTIN, ledStatus ? HIGH : LOW);
  
  delay(1000);
}
