#ifndef MQTT_h
#define MQTT_h

#include "MQTT.h"
#include "topics.h"
#include "env.h"
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient
                          // Some aspects of this code has been shamelessly copied from this repo

#include <ESP8266WiFi.h> // Included in arduino ESP8266 core

unsigned long timeLastConnectionAttempt = 0;
unsigned long reconnectInterval = 5000;

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
	if (!ensureConnection()) { return; }
	client.loop();
  delay(500); // If we remove this, the client sometimes gets disconnected
}


bool MQTT::ensureConnection() {
	if (client.connected()) { return true; } // Connection is ensured

	if (millis() - timeLastConnectionAttempt < reconnectInterval) { return false; } // Wait before reconnecting (this is non-blocking!)
	timeLastConnectionAttempt = millis(); // Time to retry!
	Serial.print("Attempting MQTT connection... ");
  delay(2000); // If we lower this value, reconnecting does not work as consistent (or not at all)
               // Also note, that the AP must have been off for at least 15 seconds in order to be able to reconnect

  // Try to connect with credentials, and declare Last Will and Testament of "false" to TOPIC_ONLINE with QoS 1 and retainment.
  yield(); // Just to be sure
	if (client.connect("Cute little plant", USERNAME, PASSWORD, TOPIC_ONLINE, 1, true, "false")) {
		// On success, resubscribe to everything and publish that device turned online again
    yield(); // Just to be sure
		client.subscribe(TOPIC_WATER);
		client.subscribe(TOPIC_SENSE);
		client.publish(TOPIC_ONLINE, "true", true); // retained
		Serial.println("Connected!");
		return true;
	}

  // Failed to connect
  Serial.print("Connection failed, will retry soon. Error code: ");
  Serial.println(client.state());
	return false;
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
    if(receivedPayload == "false")
    {
      setManual(false);
    }
    else
    {
      setManual(true);
    }
  }
}


// PUBLISH METHODS -------------------------------------------
void MQTT::publishMoist(int moist) {
  client.publish(TOPIC_MOIST, String(moist).c_str());
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
