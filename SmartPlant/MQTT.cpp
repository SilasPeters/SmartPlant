#ifndef MQTT_h
#define MQTT_h

#include "MQTT.h"
#include "topics.h"
#include "env.h"
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient
                          // Some aspects of this code has been shamelessly copied from this repo

#include <ESP8266WiFi.h> // Included in arduino ESP8266 core

PubSubClient client;

void (*waterPlant)();
void (*publishValues)();
void (*calibrateMoist)(int);
void (*setManual)(bool);

MQTT::MQTT(WiFiClient& wifiClient, void (*waterPlantRef)(), void (*publishValuesRef)(), void (*calibrateMoistRef)(int), void (*setManualRef)(bool)) : client(wifiClient) {
	client.setServer(MQTT_SERVER, 1883);
	client.setCallback(callback);

	waterPlant = waterPlantRef;
  publishValues = publishValuesRef;
	calibrateMoist = calibrateMoistRef;
  setManual = setManualRef;
}

void MQTT::loop() {
	if (!client.connected()) {
		reconnect(); // Will also be called to lay initial connection
	  announce();
	}
	client.loop();
  delay(1000); // TODO this is here just to ensure the client does not get disconnected
}

void MQTT::announce(){
  client.subscribe(TOPIC_WATER);
  client.subscribe(TOPIC_SENSE);
  client.subscribe(TOPIC_CALIBRATE_MOIST);  
  client.publish(TOPIC_ONLINE, "true", true);
  publishManual(false);
}

void MQTT::callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");

	String receivedPayload = "";
	for (int i = 0; i < length; i++) {
	  receivedPayload += (char)payload[i];
	}

	Serial.println(receivedPayload);

  if (String(topic) == TOPIC_WATER) {
		waterPlant();
  }
  if (String(topic) == TOPIC_SENSE) {
    publishValues();
  }
	if (String(topic) == TOPIC_CALIBRATE_MOIST) {
		calibrateMoist(int(payload));
	}
  if (String(topic) == TOPIC_MANUAL)
  {
    if(receivedPayload == "true")
    {
      setManual(false);
    }
    else
    {
      setManual(true);
    }
  }
}

void MQTT::reconnect() {  // TODO blocking
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect("Cute little plant", USERNAME, PASSWORD, TOPIC_ONLINE, 1, true, "false")) {
			Serial.println("connected");
		} else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println("will try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}



// PUBLISH METHODS -------------------------------------------
void MQTT::publishMoist(int moist) {
  client.publish(TOPIC_MOIST, String(moist).c_str()); // TODO what must be persistent?
}

void MQTT::publishLight(int light) {
  client.publish(TOPIC_LIGHT, String(light).c_str());
}

void MQTT::publishPressure(float pressure) {
  client.publish(TOPIC_PRESSURE, String(pressure).c_str());
}

void MQTT::publishTemperature(float temperature) {
  client.publish(TOPIC_TEMPERATURE, String(temperature).c_str());
}

void MQTT::publishTimeSinceLastWatering(int minutes) {
  client.publish(TOPIC_TIME_SINCE_LAST_WATERING, String(minutes).c_str());
}


void MQTT::publishManual(bool manual) {
  client.publish(TOPIC_MANUAL, manual ? "true" : "false", true);
}
// -----------------------------------------------------------

#endif
