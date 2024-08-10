#include "mqtt.hpp"
#include "settings.hpp"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <RTCLib.h>

#define TOPIC_PREFIX "esp32_minhnhat_nguyenlong"
#define TOPIC_SENSORS TOPIC_PREFIX "/sensors"
#define TOPIC_SETTINGS TOPIC_PREFIX "/settings"
#define TOPIC_SETTINGS_SET TOPIC_PREFIX "/settings/set"
#define MQTT_SERVER "broker.hivemq.com" 

const int settingsSyncInterval = 1000;
WiFiClient espClient;
PubSubClient client(espClient);

uint32_t lastSend = 0;
uint32_t settingLastSend = 0;

void mqttCallback(const char* topic, byte* payload, unsigned int length)
{
  auto& settings = Settings::instance();
  if (strcmp(topic, TOPIC_SETTINGS_SET) == 0)
  {
    JsonDocument doc;
    deserializeJson(doc, payload, length);

    if (doc.containsKey("timeZone"))
    {
      Serial.println("Set new timezone");
      settings.setTimeZone(doc["timeZone"].as<int>());
    }
    if (doc.containsKey("alarm"))
    {
      Serial.println("Set new alarm");
      settings.setAlarm(doc["alarm"].as<int>());
    }
    if (doc.containsKey("brand"))
    {
      Serial.println("Set new brand");
      settings.setBrand(static_cast<Brand>(doc["brand"].as<int>()));
    }
  }
}

void connectMqtt() {
  client.setServer(MQTT_SERVER, 1883);
  client.setKeepAlive(90);
  String clientId = "TestClientWhatever";
  clientId += String(random(0xffff), HEX);
  if (client.connect(clientId.c_str()))
  {
    Serial.println("Connected to MQTT");
    sendSettings();
    client.setCallback(mqttCallback);
    client.subscribe(TOPIC_SETTINGS_SET);
  }
  else
  {
    Serial.println("Failed to connect to MQTT");
  }
}

unsigned int intervalMqtt = 500;
unsigned int lastCheckMqtt = 0;
void updateMqtt()
{
  auto now = millis();
  if (!client.connected())
    connectMqtt();
  if (now - lastCheckMqtt < intervalMqtt)
    return;
  lastCheckMqtt = now;
  client.loop();
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

void sendSettings()
{
  if (millis() - settingLastSend < settingsSyncInterval)
    return;
  settingLastSend = millis();
  auto& settings = Settings::instance();
  JsonDocument doc;
  doc["timeZone"] = settings.timeZone();
  doc["alarm"] = settings.alarm();
  doc["brand"] = static_cast<int>(settings.brand());

  String jsonData;
  serializeJson(doc, jsonData);
  client.publish(TOPIC_SETTINGS, jsonData.c_str());
}
