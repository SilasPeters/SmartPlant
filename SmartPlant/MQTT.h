#include <Arduino.h>
#include <ESP8266WiFi.h> // Included in arduino ESP8266 core
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient

class MQTT
{
	public:
		MQTT(WiFiClient& wifiClient, void (*waterPlantRef)(), void (*publishValuesRef)());
		void loop();
    void publishMoist(int moist);
    void publishLight(int light);
    void publishPressure(float pressure);
    void publishTemperature(float temperature);
    void publishManual(bool manual);
    
	private:
    PubSubClient client;
    static void callback(char* topic, byte* payload, unsigned int length);
    void reconnect();
    void announce();
};
