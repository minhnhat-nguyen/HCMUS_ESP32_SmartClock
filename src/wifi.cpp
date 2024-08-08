#include "wifi.hpp"
#include <WiFi.h>

void connectWifi() {
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED)
    delay(250);
}
