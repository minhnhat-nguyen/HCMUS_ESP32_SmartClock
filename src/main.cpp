#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <DHT.h>
#include "lib/IR/IRremote.hpp"
#include "lib/IR/IRremoteInt.h"

#include "wifi.hpp"
#include "screen.hpp"
#include "rtc.hpp"
#include "sensors.hpp"
#include "mqtt.hpp"
#include "neopixel.hpp"

#define DHT_PIN 14
#define PUSHBUTTON 16
#define BUZZER 23


#define LED 32
#define IR 25

const int interval = 1000;
const int buttonInterval = 200;
const int lcdRefreshCycle = 60;

String packedData;
DateTime currentTime;
IRsend irsend(IR);

int cycle = 0;
int lastUpdate = 0;
int lastButtonUpdate = 0;
bool alarmSwitch = false;
decode_type_t currentBrand = SONY;

char daysOfTheWeek[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
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

void sendIR() {
  analogWrite(IR, 255);
  switch (currentBrand) {
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
  analogWrite(IR, 0);
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
  // if (millis() - lastUpdate > interval) {
  //   currentTime = rtc.now();
  //   temperature = dht.readTemperature();
  //   humidity = dht.readHumidity();
  //   lastUpdate = millis();
  //   presence = digitalRead(PIR_PIN) == HIGH;
  //   refreshDisplay(currentTime, temperature, humidity);
  //   digitalWrite(LED, (presence) ? HIGH : LOW);
  //   update_NeoPixel(temperature, humidity, co2);
  //   if (++cycle == lcdRefreshCycle) {
  //     cycle = 0;
  //     lcd.clear();
  //     Serial.println(packData(currentTime, temperature, humidity, digitalRead(PIR_PIN) == HIGH));
  //   }
  // }

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