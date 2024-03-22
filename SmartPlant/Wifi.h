#include <Arduino.h>
#include <ESP8266WiFi.h> // Included in arduino ESP8266 core

class Wifi
{
  public:
    void setup(String ap_ssid, String ap_password);
};
