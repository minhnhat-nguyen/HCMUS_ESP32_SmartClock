#include "neopixel.hpp"
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL 26
#define NEOPIXEL_TEMP 0
#define NEOPIXEL_HUMID 1
#define NEOPIXEL_CO2 2

Adafruit_NeoPixel strip(3, NEOPIXEL, NEO_GRB + NEO_KHZ800);

void initNeoPixel()
{
  strip.begin();
  strip.show();
  strip.setBrightness(255);
}

void updateNeoPixel(const SensorData& data) {
  int color = map(data.temperature, -40, 80, 255, 0);
  strip.setPixelColor(NEOPIXEL_TEMP, strip.Color(255 - color, 0, color));
  color = map(data.humidity, 0, 100, 0, 255);
  strip.setPixelColor(NEOPIXEL_HUMID, strip.Color(255 - color, 0, color));
  color = map(data.co2, 0, 5500, 255, 0);
  strip.setPixelColor(NEOPIXEL_CO2, strip.Color(255 - color, 0, color));
  strip.show();
}
