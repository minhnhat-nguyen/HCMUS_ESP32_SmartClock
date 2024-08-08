#pragma once

#include "sensors.hpp"
#include <RTClib.h>

void connectMqtt();
void sendSensorData(const DateTime& timestamp, const SensorData& data);
