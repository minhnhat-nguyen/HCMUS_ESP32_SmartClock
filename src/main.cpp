#include "wifi.hpp"
#include "screen.hpp"
#include "rtc.hpp"
#include "sensors.hpp"
#include "mqtt.hpp"
#include "neopixel.hpp"
#include "lib/IR/IRremote.h"
#include "lib/IR/IRremoteInt.h"
#include "settings.hpp"

#define PUSHBUTTON 16
#define BUZZER 23
#define LED 32
#define PIN_IR 25

IRsend irsend;
int lastButtonUpdate = 0;
bool alarmSwitch = false;
decode_type_t currentBrand = SONY;

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

void sendIR(decode_type_t brand)
{
  analogWrite(PIN_IR, 255);
  switch (brand) {
    case SAMSUNG:
      irsend.sendSAMSUNG(0xE0E040BF, 32);
      break;
    case LG:
      irsend.sendLG(0x88, 32, 3);
      break;
    case SONY:
      irsend.sendSony(0xa90, 3, 3);
      break;
    case PANASONIC:
      irsend.sendPanasonic(0x4004, 16, 3);
      break;
    case SHARP:
      irsend.sendSharp(0x40, 16, 3);
      break;
  }
  analogWrite(PIN_IR, 0);
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
  irsend.begin(PIN_IR);
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