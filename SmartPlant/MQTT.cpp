#ifndef MQTT_h
#define MQTT_h

#include "MQTT.h"
#include "topics.h"
#include "env.h"
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient
                          // Some aspects of this code has been shamelessly copied from this repo

#include <ESP8266WiFi.h> // Included in arduino ESP8266 core

PubSubClient client;

unsigned long timeLastConnectionAttempt;
unsigned int reconnectInterval;

void (*waterPlant)();
void (*publishValues)();

MQTT::MQTT(WiFiClient& wifiClient, void (*waterPlantRef)(), void (*publishValuesRef)(), unsigned int reconnectIntervalParam) : client(wifiClient) {
	client.setServer(MQTT_SERVER, 1883);
	client.setCallback(callback);

	waterPlant = waterPlantRef;
  publishValues = publishValuesRef;

	timeLastConnectionAttempt = 0;
	reconnectInterval = reconnectIntervalParam;
}

void MQTT::loop() {
	if (!ensureConnection()) { return; }
	client.loop();
  delay(1000); // TODO this is here just to ensure the client does not get disconnected
}

bool MQTT::ensureConnection() {
	if (client.connected()) { return true; } // Connection is ensured

	if (millis() - timeLastConnectionAttempt < reconnectInterval) { return false; } // Wait before reconnecting
	timeLastConnectionAttempt = millis(); // Time to retry!
	Serial.print("Attempting MQTT connection... ");

  // Try to connect with credentials, and declare Last Will and Testament of "false" to TOPIC_ONLINE with QoS 1 and retainment.
	if (client.connect("Cute little plant", USERNAME, PASSWORD, TOPIC_ONLINE, 1, true, "false")) {
		// On success, resubscribe to everything and publish that device turned online again
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


void MQTT::publishManual(bool manual) {
  client.publish(TOPIC_MANUAL, manual ? "true" : "false");
}
// -----------------------------------------------------------

#endif
