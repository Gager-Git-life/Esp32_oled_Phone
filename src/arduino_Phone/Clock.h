/*
   NTP Clock Program -- 网络时钟
   Network Time Protocol (NTP)
   ESP8266 连接到NET WORK ，获取NTP
   https://github.com/YFROBOT-TM/YFRobot-NTPClock_OneNet
  library:
   https://github.com/PaulStoffregen/Time
   https://github.com/JChristensen/Timezone
   by: yfrobot
   http://www.yfrobot.com
*/
#include <Time.h>
#include <Timezone.h>
#include "Public.h"
#include "NTP.h"

// Set your WiFi login credentials
#define WIFI_SSID "TP-LINK_3ADB"       // 使用时请修改为当前你的 wifi ssid
#define WIFI_PASS "oyj961014"   // 使用时请修改为当前你的 wifi 密码

//#define ledPin 14                          // 定义ledPin连接到GPIO14

// This clock is in the Mountain Time Zone
// Change this for your timezone
// 北京时间时区
#define STD_TIMEZONE_OFFSET +8    // Standard Time offset (-7 is mountain time)

// ***************************************************************
// TimeZone and Daylight Savings Time Rules
// ***************************************************************

// Define daylight savings time rules for the China
TimeChangeRule mySTD = {"", First,  Sun, Jan, 0, STD_TIMEZONE_OFFSET * 60};
Timezone myTZ(mySTD, mySTD);

// Previous seconds value
//time_t previousSecond = 0;

int years = 0,months = 0,days = 0,weekdays = 0,
    hours = 0,minutes = 0,seconds = 0, time_record = 0;

bool Date_flg = false;

// This function is called once a second
void updateDisplay(bool control) {
  TimeChangeRule *tcr;        // Pointer to the time change rule
  // Read the current UTC time from the NTP provider
  time_t utc = now();
  // Convert to local time taking DST into consideration
  time_t localTime = myTZ.toLocal(utc, &tcr);
  // Map time to pixel positions
  weekdays=   weekday(localTime);
  days    =   day(localTime);
  months  =   month(localTime);
  years   =   year(localTime);
  seconds =   second(localTime);
  minutes =   minute(localTime);
  hours   =   hour(localTime) ;   //12 hour format use : hourFormat12(localTime)  isPM()/isAM()
  if(control){
    Serial.println("");
    Serial.print("Current local time:");
    Serial.print(days);
    Serial.print("/");
    Serial.print(months);
    Serial.print("/");
    Serial.print(years);
    Serial.print(" - ");
    Serial.print(hours);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(":");
    Serial.print(seconds);
    Serial.print(" - ");
    Serial.print(dayStr(weekdays));
    Serial.println("");
  }
}



void Clock_Automatic(){
  WiFiClient client;
  initNTP(WIFI_SSID, WIFI_PASS);
  updateDisplay(true);
  time_record = millis();
//  //  Update the display only if time has changed
//  if (timeStatus() != timeNotSet) {
//    if (second() != previousSecond) {
//      previousSecond = second();
//      // Update the display
//      updateDisplay();
//    }
//  }
//  delay(1000); 
}


