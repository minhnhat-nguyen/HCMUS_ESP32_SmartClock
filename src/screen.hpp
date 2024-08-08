#pragma once

#include "sensors.hpp"
#include <RTClib.h>

void initDisplay();
void refreshDisplay(const DateTime& timestamp, const SensorData& sensorData);
