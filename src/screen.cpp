#include "screen.hpp"
#include "settings.hpp"
#include <LiquidCrystal_I2C.h>

#define LCD_COL 20
#define LCD_ROW 4

LiquidCrystal_I2C lcd(0x27, LCD_COL, LCD_ROW);

int lastRefresh = 0;
const int refreshInterval = 200;
const char daysOfTheWeek[7][10] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

void initDisplay()
{
  lcd.init();
}

char lineBuffer[LCD_COL + 1] = {0};

void printSpace(int num)
{
  if (num == 0)
    return;
  memset(lineBuffer, ' ', num);
  lineBuffer[num] = 0;
  lcd.print(lineBuffer);
}

void refreshDisplay(DateTime timestamp, const SensorData& sensorData)
{
  int len;
  if (millis() - lastRefresh > refreshInterval) {  
    lastRefresh = millis();    
    auto& adjust = Settings::instance().timeZoneAdjustment();
    if (adjust.negative)
      timestamp = timestamp - adjust.offset;
    else 
      timestamp = timestamp + adjust.offset;

    lcd.setCursor(0, 0);
    len = snprintf(lineBuffer, sizeof(lineBuffer), "%s %d/%d/%d",
      daysOfTheWeek[timestamp.dayOfTheWeek()],
      timestamp.day(),
      timestamp.month(),
      timestamp.year()
    );
    lcd.print(lineBuffer);
    printSpace(LCD_COL - len);

    lcd.setCursor(0, 1);
    len = snprintf(lineBuffer, sizeof(lineBuffer), "%02d:%02d:%02d",
      timestamp.hour(),
      timestamp.minute(),
      timestamp.second()
    );
    lcd.print(lineBuffer);
    printSpace(LCD_COL - len);

    lcd.setCursor(0, 2);
    len = snprintf(lineBuffer, sizeof(lineBuffer), "%.2fC  %.2f%%",
      sensorData.temperature,
      sensorData.humidity
    );
    lcd.print(lineBuffer);
    printSpace(LCD_COL - len);

    lcd.setCursor(0, 3);
    len = snprintf(lineBuffer, sizeof(lineBuffer), "CO2: %.2fppm",
      sensorData.co2);
    lcd.print(lineBuffer);
    printSpace(LCD_COL - len);
    lcd.noCursor();
  }
}
