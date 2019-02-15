#include <Wire.h>
#include <string.h>
#include "SSD1306.h"
#include "OLEDDisplayUi.h"


#define Esp32

#ifdef Esp8266
  #include "ESP8266WiFi.h"
#endif
#ifdef Esp32
  #include <WiFi.h>
#endif









