#pragma once

struct SensorData {
  float temperature;
  float humidity;
  float co2;
  bool presence;
};

void initSensors();
void readSensors(SensorData &data);
