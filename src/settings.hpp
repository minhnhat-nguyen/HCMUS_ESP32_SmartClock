#pragma once

#include "ir.hpp"
#include <RTClib.h>

struct TimeZoneAdjustment
{
  TimeSpan offset;
  bool negative;
};

class Settings
{
public:
  static Settings& instance()
  {
    static Settings instance;
    return instance;
  }
  Settings(const Settings&) = delete;
  void operator=(const Settings&) = delete;

  int alarm() const;
  void setAlarm(int alarm);

  int timeZone() const;
  void setTimeZone(int timeZone);

  const TimeZoneAdjustment& timeZoneAdjustment() const;

  Brand brand() const;
  void setBrand(Brand brand);

private:
  Settings();
  void _setTimeZoneAdjustment();

  int _alarm;
  int _timeZone;
  TimeZoneAdjustment _timeZoneAdjustment;
  Brand _brand;
};
