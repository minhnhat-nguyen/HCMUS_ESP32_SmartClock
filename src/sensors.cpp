#include "sensors.hpp"
#include "sensors_priv.hpp"

void initSensors()
{
  initCO2();
  initDHT();
  initPIR();
}

void readSensors(SensorData& data)
{
  readCO2(data);
  readDHT(data);
  readPIR(data);
}
