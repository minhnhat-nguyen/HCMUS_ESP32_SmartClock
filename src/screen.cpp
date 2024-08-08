#include "screen.hpp"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const char daysOfTheWeek[7][10] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

void initDisplay()
{
  lcd.init();
}

uint32_t lastClear = 0;
void refreshDisplay(const DateTime &timestamp, const SensorData &sensorData)
{
  auto now = millis();
  if (now - lastClear > 5000)
  {
    lcd.clear();
    lastClear = now;
  }
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[timestamp.dayOfTheWeek()]);
  lcd.print(" ");
  lcd.print(timestamp.day(), DEC);
  lcd.print('/');
  lcd.print(timestamp.month(), DEC);
  lcd.print('/');
  lcd.print(timestamp.year(), DEC);
  lcd.setCursor(0, 1);
  lcd.print(timestamp.hour(), DEC);
  lcd.print(':');
  lcd.print(timestamp.minute(), DEC);
  lcd.print(':');
  lcd.print(timestamp.second(), DEC);
  lcd.setCursor(0, 2);
  lcd.print(sensorData.temperature);
  lcd.print("C  ");
  lcd.print(sensorData.humidity);
  lcd.print("%");
  lcd.setCursor(0, 3);
  lcd.print("CO2: ");
  lcd.print(sensorData.co2);
  lcd.print("ppm");
  lcd.noCursor();
}
