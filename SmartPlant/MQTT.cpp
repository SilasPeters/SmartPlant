#ifndef MQTT_h
#define MQTT_h

#include "MQTT.h"
#include "env.h"
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient
                          // Some aspects of this code has been shamelessly copied from this repo

#include <ESP8266WiFi.h> // Included in arduino ESP8266 core

const String logTopic = "infob3it/" + String(USERNAME) + "/log";
const String announcementTopic = "infob3it/" + String(USERNAME) + "/announce";

PubSubClient client;
String lastReceivedMessage;

MQTT::MQTT(WiFiClient& wifiClient) : client(wifiClient) {
	client.setServer(MQTT_SERVER, 1883);
	client.setCallback(callback);

	lastReceivedMessage = "";
}

void MQTT::loop() {
	if (!client.connected()) {
		reconnect(); // Will also be called to lay initial connection
	  announce();
	}
  // Echo latest message
  if (lastReceivedMessage.length() > 0) {
	  client.publish(logTopic.c_str(), lastReceivedMessage.c_str());
	  lastReceivedMessage = "";
  }
	client.loop();
}

void MQTT::announce(){
  client.subscribe(announcementTopic.c_str());
  client.publish(logTopic.c_str(), "*ribbit*", true); // Retained
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
	
	lastReceivedMessage = receivedPayload;
}

void MQTT::reconnect() {  // TODO blocking
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect("Cute little plant", USERNAME, PASSWORD)) {
			Serial.println("connected");
		} else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

#endif
