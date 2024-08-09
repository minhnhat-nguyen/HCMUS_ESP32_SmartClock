#include "rtc.hpp"
#include <NTPClient.h>
#include <WiFi.h>
#include <WifiUdp.h>

RTC_DS1307 rtc;
WiFiUDP udp;
NTPClient ntp(udp, "pool.ntp.org", 0, 0);
DateTime lastCheck = DateTime("0");

void updateNTP()
{
  while (!ntp.update())
  {
    Serial.println("Failed to update NTP time");
  }
}

void initRTC()
{
  rtc.begin();
  ntp.begin();
  updateNTP();
  unsigned long epoch = ntp.getEpochTime();
  rtc.adjust(DateTime(epoch));
  lastCheck = rtc.now();
}


DateTime readRTC()
{
  auto now = rtc.now();
  if ((now - lastCheck).days() >= 1)
  {
    updateNTP();
    rtc.adjust(DateTime(ntp.getEpochTime()));
    lastCheck = now;
    return rtc.now();
  }
  return now;
}
