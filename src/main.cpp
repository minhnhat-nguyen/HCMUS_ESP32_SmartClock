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
#define buttonInterval 100

int lastButtonUpdate = 0;
bool alarmSwitch = false;

DateTime alarmTime = DateTime("2000-00-00T16:10:00");


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

SensorData data{
  .temperature = 0,
  .humidity = 0,
  .co2 = 0,
  .presence = false
};

void loop() {
  readSensors(data);
  auto timestamp = readRTC();
  refreshDisplay(timestamp, data);
  sendSensorData(timestamp, data);
  digitalWrite(LED, data.presence);
  updateNeoPixel(data);
  /*if (currentTime.hour() == alarmTime.hour() && currentTime.minute() == alarmTime.minute()) {
      if (!alarmSwitch) {
        alarmSwitch = true;
        playAlarm();
      }
    }
    else {
      alarmSwitch = false;
    } */

   if (digitalRead(PUSHBUTTON) == HIGH) {
     if (millis() - lastButtonUpdate > buttonInterval) {
       lastButtonUpdate = millis();
       toneBuzzer(1000, 100);
       sendIR(Brand::LG);
     }
   }
}