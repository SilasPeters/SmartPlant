#ifndef Wifi_h
#define Wifi_h

#include "Wifi.h"
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266WiFi.h> // Included in arduino ESP8266 core

WiFiManager wifiManager;

void Wifi::setup(String ap_ssid, String ap_password) { // TODO make this static?
  wifiManager.autoConnect(ap_ssid.c_str(), ap_password.c_str());
}

#endif
