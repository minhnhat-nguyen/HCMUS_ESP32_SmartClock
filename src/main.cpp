#include "wifi.hpp"
#include "screen.hpp"
#include "rtc.hpp"
#include "sensors.hpp"
#include "mqtt.hpp"
#include "neopixel.hpp"
#include "ir.hpp"
#include "settings.hpp"
#include "buzzer.hpp"

#define PUSHBUTTON 16
#define LED 32
#define buttonInterval 50

int lastButtonUpdate = 0;

void checkButtonTrigger() {
  if (millis() - lastButtonUpdate > buttonInterval) {
    if (digitalRead(PUSHBUTTON) == HIGH) {
      lastButtonUpdate = millis();
      toneBuzzer(1000, 150);
      sendIR(Settings::instance().brand());
    }
  }
}

SensorData data{
  .temperature = 0,
  .humidity = 0,
  .co2 = 0,
  .presence = false
};

void setup() {
  Serial.begin(9600);

  connectWifi();
  connectMqtt();

  Wire.begin();

  pinMode(LED, OUTPUT);
  pinMode(PUSHBUTTON, INPUT);

  initBuzzer();
  initDisplay();
  initSensors();
  initRTC();
  initNeoPixel();
  initIR();
  Serial.println("Setup done!");
}

void loop() {
  updateMqtt();
  readSensors(data);
  auto timestamp = readRTC();
  refreshDisplay(timestamp, data);
  sendSensorData(timestamp, data);
  sendSettings();
  digitalWrite(LED, data.presence);
  updateNeoPixel(data);
  checkAlarmTime(timestamp);
  checkButtonTrigger();
}