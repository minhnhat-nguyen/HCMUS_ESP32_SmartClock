#include "settings.hpp"
#include <Preferences.h>
#include <nvs_flash.h>

#define KEY_ALARM "alarm"
#define KEY_TIMEZONE "timeZone"
#define KEY_BRAND "brand"


Preferences prefs;

Settings::Settings()
{
  nvs_flash_init();
  prefs.begin("settings");
  _alarm = prefs.getInt(KEY_ALARM, -1);
  _timeZone = prefs.getInt(KEY_TIMEZONE, 700);
  _setTimeZoneAdjustment();
  _brand = static_cast<Brand>(prefs.getInt(KEY_BRAND));
}

int Settings::alarm() const
{
  return _alarm;
}

void Settings::setAlarm(int alarm)
{
  _alarm = alarm;
  prefs.putInt(KEY_ALARM, alarm);
}

int Settings::timeZone() const
{
  return _timeZone;
}

void Settings::setTimeZone(int timeZone)
{
  _timeZone = timeZone;
  _setTimeZoneAdjustment();
  prefs.putInt(KEY_TIMEZONE, timeZone);
}

const TimeZoneAdjustment& Settings::timeZoneAdjustment() const
{
  return _timeZoneAdjustment;
}

void Settings::_setTimeZoneAdjustment()
{
  _timeZoneAdjustment.negative = _timeZone < 0;
  _timeZoneAdjustment.offset = TimeSpan(0, abs(_timeZone) / 100, abs(_timeZone % 100), 0);
}

Brand Settings::brand() const
{
  return _brand;
}

void Settings::setBrand(Brand brand)
{
  _brand = brand;
  prefs.putInt(KEY_BRAND, static_cast<int32_t>(brand));
}
