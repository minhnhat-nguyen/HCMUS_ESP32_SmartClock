#include "screen.hpp"
#include "settings.hpp"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
int timeZone = Settings::instance().timeZone();
TimeSpan adjust(0, abs(timeZone) / 100, abs(timeZone % 100), 0);
bool _reset = false;
int lastRefresh = 0;

const int refreshInterval = 200;
const char daysOfTheWeek[7][10] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

void initDisplay()
{
  lcd.init();
}


void refreshDisplay(DateTime timestamp, const SensorData& sensorData)
{
  if (millis() - lastRefresh > refreshInterval) {  
    lastRefresh = millis();    
    timeZone = Settings::instance().timeZone();
    adjust = TimeSpan(0, abs(timeZone) / 100, abs(timeZone % 100), 0);
    if (timeZone > 0)
      timestamp = timestamp + adjust;
    else 
      timestamp = timestamp - adjust;
    
    if (timestamp.second() == 0 && !_reset) 
    {
      lcd.clear();
      _reset = true;
    }
    else 
      _reset = false;

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
}
