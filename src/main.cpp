#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "IR/IRremote.hpp"
#include "IR/IRremoteInt.h"

#define DHT_pin 14
#define pushButton 16
#define buzzer 23
#define NEOPIXEL 26
#define NEOPIXEL_temp 0
#define NEOPIXEL_humid 1
#define NEOPIXEL_co2 2
#define PIR_pin 27
#define LED 32
#define MQ135_pin 34
#define PARA 116.6020682
#define PARB 2.769034857
#define IR 25

const float rzero=76.63;
const float rload=10.0;
const int interval = 1000;
const int buttonInterval = 200;
const int lcdRefreshCycle = 60;

LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS1307 rtc;
DHT dht(DHT_pin, DHT22);
String packedData;
DateTime currentTime;
PubSubClient client;
IRsend irsend(IR);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, NEOPIXEL, NEO_GRB + NEO_KHZ800);

int cycle = 0;
int lastUpdate = 0;
int lastButtonUpdate = 0;
float temperature;
float humidity;
int co2 = 100;
bool presence = false;
bool alarmSwitch = false;
decode_type_t currentBrand = SONY;

char daysOfTheWeek[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
DateTime alarmTime = DateTime("2000-00-00T16:10:00");


void connectWifi() {
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED)
    delay(250);
}

void connectMqtt() {
  client.setServer("broker.hivemq.com", 1883);
  client.connect("esp32_minhnhat_nguyenlong");
}

void playAlarm() {
  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  int noteDurations[] = {500, 500, 500, 500, 500, 500, 500, 500};
  for (int i = 0; i < 8; i++) {
    tone(buzzer, melody[i], noteDurations[i]);
    delay(50);
    noTone(buzzer);
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

float getPPM() {
  int val = analogRead(MQ135_pin);
  float adjusted_val = (float)val * (1023.0 / 4095.0); // not neccesary for the actual mq135 but only for the simulation
  float rs = ((1023.0 / adjusted_val) - 1.0) * rload;  
  float ppm = PARA * pow((rs / rzero), -PARB);
  return map(ppm, 0, max(ppm, (float)65000), 350, 5500); // the map function is only for the simulation, remove it for the actual mq135
}

void refreshDisplay(const DateTime& now, const float& temperature, const float& humidity) {
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.print(" ");
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  lcd.setCursor(0, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  lcd.setCursor(0, 2);
  lcd.print(temperature);
  lcd.print("C  ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 3);
  lcd.print("CO2: ");
  lcd.print(co2);
  lcd.print("ppm");
  lcd.noCursor();
}

void update_NeoPixel(const float& temperature, const float& humidity, const int& co2) {
  int color = map(temperature, -40, 80, 255, 0);
  strip.setPixelColor(NEOPIXEL_temp, strip.Color(255 - color, 0, color));
  color = map(humidity, 0, 100, 0, 255);
  strip.setPixelColor(NEOPIXEL_humid, strip.Color(255 - color, 0, color));
  color = map(co2, 0, 5500, 255, 0);
  strip.setPixelColor(NEOPIXEL_co2, strip.Color(255 - color, 0, color));
  strip.show();
}

JsonDocument fetchData() {
  JsonDocument json;
  return json;
}

const char* packData(const DateTime& timestampt, 
                    const float& temperature,
                    const float& humidity,
                    const bool presence) {
  JsonDocument json;
  json["timestampt"] = timestampt.timestamp();
  json["temperature"] = temperature;
  json["humidity"] = humidity;
  json["presence"] = presence;
  json["co2"] = co2;
  serializeJson(json, packedData);
  return packedData.c_str();
}

void setup() {
  Serial.begin(9600);
  connectWifi();

  ledcSetup(0, 5000, 8);
  ledcAttachPin(buzzer, 0);
  Wire.begin();
  dht.begin();
  lcd.init();
  rtc.begin();
  strip.begin();
  strip.show();
  strip.setBrightness(255);

  pinMode(LED, OUTPUT); 
  pinMode(PIR_pin, INPUT);
  pinMode(pushButton, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  if (millis() - lastUpdate > interval) {
    currentTime = rtc.now();
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    lastUpdate = millis();
    presence = digitalRead(PIR_pin) == HIGH;
    refreshDisplay(currentTime, temperature, humidity);
    digitalWrite(LED, (presence) ? HIGH : LOW);
    co2 = getPPM();
    update_NeoPixel(temperature, humidity, co2);
    if (++cycle == lcdRefreshCycle) {
      cycle = 0;
      lcd.clear();
      Serial.println(packData(currentTime, temperature, humidity, digitalRead(PIR_pin) == HIGH));
    }
  }

  if (currentTime.hour() == alarmTime.hour() && currentTime.minute() == alarmTime.minute()) {
    if (!alarmSwitch) {
      alarmSwitch = true;
      playAlarm();
    }
  }
  else {
    alarmSwitch = false;
  }

  if (digitalRead(pushButton) == HIGH) {
    if (millis() - lastButtonUpdate > buttonInterval) {
      lastButtonUpdate = millis();
      tone(buzzer, 1000, 100);
      sendIR();
    }
  }
}