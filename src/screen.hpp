#pragma once

#include "sensors.hpp"
#include <RTClib.h>

void initDisplay();
void refreshDisplay(DateTime timestamp, const SensorData& sensorData);
