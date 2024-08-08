#pragma once

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

private:
  Settings() {}

  int _alarm;
  int _timeZone;
};
