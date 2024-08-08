#include "rtc.hpp"
#include <NTPClient.h>
#include <WiFi.h>
#include <WifiUdp.h>

RTC_DS1307 rtc;
WiFiUDP udp;
NTPClient ntp(udp, "pool.ntp.org", 0, 0);

void initRTC()
{
  rtc.begin();
}

DateTime lastCheck;
DateTime readRTC()
{
  auto now = rtc.now();
  if ((now - lastCheck).days() >= 1)
  {
    ntp.update();
    rtc.adjust(DateTime(ntp.getEpochTime()));
    lastCheck = now;
  }
  return now;
}
