#include "sensors_priv.hpp"
#include <Arduino.h>

#define PIR_PIN 27

void initPIR()
{
  pinMode(PIR_PIN, INPUT);
}

void readPIR(SensorData &data)
{
  data.presence = digitalRead(PIR_PIN);
}
