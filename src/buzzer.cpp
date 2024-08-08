#include "buzzer.hpp"
#include <Arduino.h>

#define BUZZER 23

const int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
const int noteDurations[] = {500, 500, 500, 500, 500, 500, 500, 500};

void initBuzzer() {
  ledcSetup(0, 5000, 8);
  ledcAttachPin(BUZZER, 0);
  pinMode(BUZZER, OUTPUT);
}

void playAlarm() {
  for (int i = 0; i < 8; i++) {
    tone(BUZZER, melody[i], noteDurations[i]);
    delay(50);
    noTone(BUZZER);
  }
}

void toneBuzzer(int frequency, int duration) {
    tone(BUZZER, frequency, duration);
}
