#include "mqtt.hpp"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <RTCLib.h>

#define TOPIC_PREFIX "esp32_minhnhat_nguyenlong"
#define TOPIC_SENSORS TOPIC_PREFIX "/sensors"
#define TOPIC_SETTINGS TOPIC_PREFIX "/settings"

WiFiClient espClient;
PubSubClient client(espClient);

uint32_t lastSend = 0;

void connectMqtt() {
  client.setServer("broker.hivemq.com", 1883);
  client.setKeepAlive(90);
  String clientId = "TestClientWhatever";
  clientId += String(random(0xffff), HEX);
  if (client.connect(clientId.c_str()))
  {
    Serial.println("Connected to MQTT");
  }
  else
  {
    Serial.println("Failed to connect to MQTT");
  }
}

void sendSensorData(const DateTime& timestamp, const SensorData& data)
{
  auto now = millis();
  if (now - lastSend < 1000)
    return;
  lastSend = now;
  JsonDocument doc;
  doc["timestamp"] = timestamp.unixtime();
  doc["temperature"] = data.temperature;
  doc["humidity"] = data.humidity;
  doc["presence"] = data.presence;
  doc["co2"] = data.co2;

  String jsonData;
  serializeJson(doc, jsonData);
  client.publish(TOPIC_SENSORS, jsonData.c_str());
}

