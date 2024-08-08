#include <Arduino.h>
#include <RTClib.h>
#include "buzzer.hpp"
#include "settings.hpp"

#define BUZZER 23

const int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
const int noteDurations[] = {500, 500, 500, 500, 500, 500, 500, 500};

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
    int alarm = Settings::instance().alarm();
    if (alarm == -1) {
        return;
    }
    int timeZone = Settings::instance().timeZone();
    TimeSpan adjust(0, abs(timeZone) / 100, abs(timeZone % 100), 0);
    if (timeZone > 0)
        currentTime = currentTime + adjust;
    else 
        currentTime = currentTime - adjust;
    if (currentTime.hour() == alarm / 100 && currentTime.minute() == alarm % 100) {
        if (!_alarmSwitch) {
            _alarmSwitch = true;
            _playAlarm();
        }
    }
    else {
        _alarmSwitch = false;
    }
}