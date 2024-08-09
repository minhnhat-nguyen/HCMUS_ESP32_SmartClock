#pragma once

#include "ir.hpp"

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

  Brand brand() const;
  void setBrand(Brand brand);

private:
  Settings();
  int _alarm;
  int _timeZone;
  Brand _brand;
};
