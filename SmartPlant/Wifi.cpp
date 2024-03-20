#ifndef Wifi_h
#define Wifi_h

#include "Wifi.h"
/* #include <ESP8266WiFi.h> // Included in ESP8266 Ardiono core */
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

WiFiManager wifiManager;

void Wifi::setup(String ap_ssid, String ap_password) {
  wifiManager.autoConnect(ap_ssid.c_str(), ap_password.c_str());
}

void Wifi::loop() {
}

#endif
