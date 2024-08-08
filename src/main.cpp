#include "wifi.hpp"
#include "screen.hpp"
#include "rtc.hpp"
#include "sensors.hpp"
#include "mqtt.hpp"
#include "neopixel.hpp"
#include "ir.hpp"
#include "settings.hpp"

#define PUSHBUTTON 16
#define BUZZER 23
#define LED 32


int lastButtonUpdate = 0;
bool alarmSwitch = false;

DateTime alarmTime = DateTime("2000-00-00T16:10:00");

void playAlarm() {
  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  int noteDurations[] = {500, 500, 500, 500, 500, 500, 500, 500};
  for (int i = 0; i < 8; i++) {
    tone(BUZZER, melody[i], noteDurations[i]);
    delay(50);
    noTone(BUZZER);
  }
}


void setup() {
  Serial.begin(9600);

  connectWifi();
  connectMqtt();

  ledcSetup(0, 5000, 8);
  ledcAttachPin(BUZZER, 0);
  Wire.begin();

  pinMode(LED, OUTPUT);
  pinMode(PUSHBUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);

  initDisplay();
  initSensors();
  initRTC();
  initNeoPixel();
  initIR();
  Settings::instance().setTimeZone(830);
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
  sendIR(Brand::SAMSUNG);
  // if (currentTime.hour() == alarmTime.hour() && currentTime.minute() == alarmTime.minute()) {
  //   if (!alarmSwitch) {
  //     alarmSwitch = true;
  //     playAlarm();
  //   }
  // }
  // else {
  //   alarmSwitch = false;
  // }

  // if (digitalRead(PUSHBUTTON) == HIGH) {
  //   if (millis() - lastButtonUpdate > buttonInterval) {
  //     lastButtonUpdate = millis();
  //     tone(BUZZER, 1000, 100);
  //     sendIR();
  //   }
  // }
}