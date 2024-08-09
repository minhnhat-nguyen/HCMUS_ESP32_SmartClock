#include <Arduino.h>
#include <RTClib.h>
#include "buzzer.hpp"
#include "settings.hpp"

#define BUZZER 23

const int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
const int noteDurations[] = {500, 500, 500, 500, 500, 500, 500, 500};

const int interval = 500;
int lastCheckAlarm = 0;
bool _alarmSwitch = false;

void initBuzzer() {
  ledcSetup(0, 5000, 8);
  ledcAttachPin(BUZZER, 0);
  pinMode(BUZZER, OUTPUT);
}

void _playAlarm() {
  for (int i = 0; i < 8; i++) {
    tone(BUZZER, melody[i], noteDurations[i]);
    delay(50);
    noTone(BUZZER);
  }
}

void toneBuzzer(int frequency, int duration) {
    tone(BUZZER, frequency, duration);
}

void checkAlarmTime(DateTime currentTime) {
    auto now = millis();
    if (now - lastCheckAlarm < interval)
        return;
    lastCheckAlarm = now;
    int alarm = Settings::instance().alarm();
    if (alarm == -1) {
        return;
    }
    auto& adjust = Settings::instance().timeZoneAdjustment();
    if (adjust.negative)
        currentTime = currentTime - adjust.offset;
    else 
        currentTime = currentTime + adjust.offset;
    if (currentTime.hour() * 100 + currentTime.minute() == alarm && !_alarmSwitch) {
        _playAlarm();
        _alarmSwitch = true;
    }
    else {
        _alarmSwitch = false;
    }
}