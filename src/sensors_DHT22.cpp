#include "sensors_priv.hpp"
#include <DHT.h>
#include <DHT_U.h>

#define DHT_PIN 14

DHT_Unified dht(DHT_PIN, DHT22);
uint32_t delayMS;
uint32_t lastRead = 0;

void initDHT()
{
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
}

void readDHT(SensorData &data)
{
  if (millis() - lastRead < delayMS)
    return;
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  data.temperature = isnan(event.temperature) ? -1 : event.temperature;

  dht.humidity().getEvent(&event);
  data.humidity = isnan(event.relative_humidity) ? -1 : event.relative_humidity;
}
