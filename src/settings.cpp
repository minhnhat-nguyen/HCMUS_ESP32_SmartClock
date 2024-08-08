#include "settings.hpp"
#include <Preferences.h>

#define KEY_ALARM "alarm"
#define KEY_TIMEZONE "timeZone"

Preferences prefs;

Settings::Settings()
{
  prefs.begin("settings");
  _alarm = prefs.getInt(KEY_ALARM, -1);
  _timeZone = prefs.getInt(KEY_TIMEZONE, 700);
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
  prefs.putInt(KEY_TIMEZONE, timeZone);
}
