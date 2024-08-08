#include "sensors_priv.hpp"
#include <MQUnifiedsensor.h>

#define MQ135_PIN 34
#define PARA 116.6020682
#define PARB 2.769034857

const float rzero = 76.63;
const float rload = 10.0;

MQUnifiedsensor mq135("ESP32", 3.3, 12, MQ135_PIN, "MQ135");

void initCO2()
{
  mq135.setRegressionMethod(1);
  mq135.setA(PARA);
  mq135.setB(PARB);
  mq135.setR0(rzero);
  mq135.setRL(rload);
  mq135.init();
}

void readCO2(SensorData &data)
{
  mq135.update();
  data.co2 = min(mq135.readSensor(), 5000.0f);
}
