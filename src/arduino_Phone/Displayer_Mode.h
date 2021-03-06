#include "Public.h"
#include "Wifi_Mode.h"
#include "Clock.h"
#include "Weather_Mode.h"
#include "DHT_Demo.h"
#include "IR_Demo.h"
#include "image.h"
#include "image_weather.h"
#include "File_Mange.h"

//SSD1306  display(0x3c, D2, D1);

//#define DEMO_DURATION 3000
typedef void (*Demo)(void);

extern String IR_Num[],Fun_Num[],Ssid[];
extern unsigned long IR_receive;
extern bool DHT_flg,IR_flg,WiFi_WPA_Flg,IR_Receive_Flg;
extern int temp,humi,tol;
extern int months,days,weekdays,hours,minutes,seconds,time_record;
extern int last_recode_time,demoMode,Sec_demoMode,Third_demoMode,Fourth_demoMode,Five_demoMode,Network_count;
extern bool Three_fun_flg,OLED_Pro_flg,Second_fun_flg ,AP_flg,Date_flg,Manul_Seting_flg,Fourth_fun_flg,Five_fun_flg;
extern char Number_str[];

extern int Weather_draw_count;
extern String Weather_Temp;
extern bool Weather_draw_flg;

int Num = 0;
String text = "";

extern String Listdir_Info[];
extern bool File_flg[];
extern bool SD_flg;
extern int Listdir_Num;
extern bool SD_init ;
extern String SD_Type;
extern String SD_Size;
extern String SD_Msg;

bool WiFi_Manul_Link = false;

String SS_ID[] = {};

int num = 0;
int num1 = 0, num2 = 0, num3 = 0,
    num4 = 0, num5 = 0, num6 = 0;

//###############UI部分的代码###############//
void Display_init(){
  // Initialising the UI will init the display too.
  //Serial.println("Displayer initeing.....");
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  delay(100);
  //Serial.println("Displayer inited");
}

void draw_Spot(int x, int y, bool sec){
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  if(sec) display.drawString(x, y, ">");  
  else display.drawString(x, y, "o");
}

void draw_nofuction(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 32, "No function!"); 
  display.display();
  delay(1500);
  Three_fun_flg = false;
}

void Time_Change(){
  seconds = seconds + (millis()- time_record)/1000;
  time_record = millis();
  if(seconds >= 60) seconds %= 60, minutes += 1;
  if(minutes >= 60) minutes %= 60, hours += 1;
  if(hours >= 24) hours %= 24;
}

void draw_IR(){
  if(IR_flg){
    IR_Start(true);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0,12,String("IR: "));
//  display.drawString(0, 12, String("IR:") + String(IR_Receive_Flg == true ? "!" : "0"));
    for(int i=0; i<3; i++){
      if(Fun_Num[i].equals(String(IR_receive,HEX))){
        Num = i+1,Serial.println(Num);
        if(Num == 0) text = "?";
        else if(Num == 1) text = "||" ;
        else if(Num == 2) text = "<<";
        else if(Num == 3) text = ">>";
        display.drawString(16,12,text);
      }
    }
    text = "",IR_receive = 0;
  }  
}

void msOverlay() {
  display.setFont(ArialMT_Plain_10);
  if(Date_flg) {
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    Time_Change();
    display.drawString(0, 0, String("D:") + String(hours) + ":" + String(minutes));// + ":" + String(seconds));
    //display.drawString(128, 12, String("WK:") + String(weekdays - 1));
  }
  else display.drawString(0, 0, String("?:?:?"));
  if(DHT_flg){
    //Weather_draw_flg = false;
    if(!Second_fun_flg && !Three_fun_flg) {
      if (((millis() - last_recode_time)/1000) > 10) DHT_Read(false);
    }
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(128, 0, String("DT: ") + String(temp));
    //display.drawString(0, 24, String("Hmi:") + String(humi));
  }

  if(Weather_draw_flg){
    //DHT_flg = false;
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(128, 12, String("WT: ") + Weather_Temp);    
  }
  
}
void fuck(){
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  if(demoMode == 8){
    draw_Spot(44,54,false);
    draw_Spot(64,54,false); 
    draw_Spot(84,54,true);
  }
  else if(demoMode == 0){
    draw_Spot(44,54,true);
    draw_Spot(64,54,false);
    draw_Spot(84,54,false);
  }
  else{
    draw_Spot(44,54,false);
    draw_Spot(64,54,true);
    draw_Spot(84,54,false);
  }
}

void OLED_protection(){
  int t = millis();
  if((t - last_recode_time)/1000 > 60){
    Serial.println("OLED Ptotection");
    OLED_Pro_flg = true;
    display.clear();
    display.display();
  }  
  else OLED_Pro_flg = false;
}




//开机显示
void draw_Start_image(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 0, "Wecome To Gager");
  display.drawXbm(0, 0, Start_Logo_width, Start_Logo_height, Start_Logo_bits);
  for(int i=0; i<500; i++){
    int progress = (i / 5) % 100;
    display.drawProgressBar(4, 54, 120, 10, progress);
    display.display();
    delay(5);
//    display.setTextAlignment(TEXT_ALIGN_CENTER);
//    display.drawString(64, 15, String(progress) + "%");
  }
  
}

//wifi显示
void draw_Wifi_image() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}
void draw_Wifi_text() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "WIFI");
}

void draw_wifi_AP(){
  Three_fun_flg = false;
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  //display.setFont(ArialMT_Plain_10); 
  display.drawString(64, 20, AP_SSID + String("  AP"));
  String text = (AP_flg == true ? String(WiFi.softAPgetStationNum()) : String("Not open"));
  display.drawString(64, 40, text);
  //display.drawString(64, 37,  String("PWD-->") + AP_PWD);
  display.display();
  delay(2000);
}
void draw_WiFi_Link(){
  WiFi_Manul_Link = true;
  WiFiClient client;
  display.setTextAlignment(TEXT_ALIGN_CENTER);  
  display.drawString(64, 10, "Input Passwd");
  display.drawString(64, 45, "OK");
  if(Fourth_demoMode != 9) draw_Spot(50 + Fourth_demoMode*5, 30, true);
  else draw_Spot(64, 55, true);
  if(Five_fun_flg){
    if(Fourth_demoMode == 9) {
      if(Five_demoMode == 0) {
        const char *p = SS_ID[Third_demoMode].c_str();
        WiFi.begin(p);
        while (WiFi.status() != WL_CONNECTED) {
          if((millis() - last_recode_time)/1000 > 10){
            display.drawString(64, 20, "Link Faild Please Try Again");
            Fourth_fun_flg = false , Fourth_demoMode = 0;
          }
          else display.drawString(64, 20, "Linking WiFi...."), Fourth_fun_flg = false , Fourth_demoMode = 0;
        }
      }
      else ;
    }
    else{  
      display.drawString(40 + Fourth_demoMode*5, 30, String(Five_demoMode));
    }
  }
}
void draw_wifi_ScanNetwork(bool displayer_flg){
  Manul_Seting_flg = true;
  if((millis()-last_recode_time)/1000 > 5) Network_count = 0;
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //display.setFont(ArialMT_Plain_10);
  if(Fourth_fun_flg){
    if(Third_demoMode == Network_count){
      Third_demoMode = 0;
      Fourth_fun_flg = false;
      Three_fun_flg = false;
    }
    else {
      draw_WiFi_Link();
    }
  }
  if(Network_count == 0){
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    //Serial.println("no networks found");
    display.drawString(64, 32, "Scanning Network ...");
    Scan_Network();
  }
  else
  {
    Serial.println(" networks found");
    for (int i = 0; i < Network_count; ++i)
    {
      // Print SSID and RSSI for each network found
      if(displayer_flg){
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        #ifdef Esp32
          WiFi_WPA_Flg = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? false : true);
        #endif
        #ifdef Esp8266
          WiFi_WPA_Flg = (WiFi.encryptionType(i) == ENC_TYPE_NONE ? false : true);
        #endif
        Serial.println((WiFi_WPA_Flg == true ? "#" : " !" ));
        if(!WiFi_WPA_Flg) SS_ID[i] = WiFi.SSID(i);
      }
      String text = String(i+1) + String(">") + ":" + String(WiFi.SSID(i)) + "(" + String(WiFi.RSSI(i)) + ")" + (WiFi_WPA_Flg == true ? "#" : " !" );
      display.drawString(0, i*12, text);
      display.drawString(0, (Network_count*12),String("<!>:Quit"));
      delay(10);
    }   
    draw_Spot(128, Third_demoMode*12, true);
    //draw_Spot(120, 54, true);
  }
  display.display();
}

void Wifi_Second_Fuc(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0,  "<1>WIFI AP");
  display.drawString(90, 0,  AP_flg == true ? String("H(") + String(WiFi.softAPgetStationNum()) + String(")"): "OFF");
  display.drawString(0, 12, "<2>WIFI Scan");
  display.drawString(0, 24, "<3>WIFI Attack");
  display.drawString(0, 36, "<4>Quit");
  draw_Spot(128, Sec_demoMode*12, true);
  if(Three_fun_flg){
    if(Sec_demoMode == 0){
      AP_flg = !AP_flg;
      if(AP_flg) Start_AP();
      else WiFi.softAPdisconnect(false);
      draw_wifi_AP();
    }
    else if(Sec_demoMode == 1) {
      draw_wifi_ScanNetwork(true);
    }
    else if(Sec_demoMode == 2) draw_nofuction();
    else if(Sec_demoMode == 3){
      Second_fun_flg = false;
      Three_fun_flg = false;
      Sec_demoMode = 0;
    }
  }

}
//蓝牙显示
void draw_Bluetooch_image() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    display.drawXbm(0, 0, Bluetooch_Logo_width, Bluetooch_Logo_height, Bluetooch_Logo_bits);
}
void draw_Bluetooch_text() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "BT");
}
void Bluetooch_Second_Fuc(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0,  "<1>Bluetooch");
  display.drawString(0, 12, "<2>Bluetooch Scan");
  display.drawString(0, 24, "<3>Bluetooch Link");
  display.drawString(0, 36, "<3>Bluetooch Send Msg");
  display.drawString(0, 48, "<4>Quit");  
  draw_Spot(128, Sec_demoMode*12, true);
  if(Three_fun_flg){
    if(Sec_demoMode == 0){
     draw_nofuction();
    }
    else if(Sec_demoMode == 1) draw_nofuction();
    else if(Sec_demoMode == 2) draw_nofuction();
    else if(Sec_demoMode == 3) draw_nofuction();
    else if(Sec_demoMode == 4){
      Second_fun_flg = false;
      Three_fun_flg = false;
      Sec_demoMode = 0;
    }
  }
}

//信息显示
void draw_Mesage_image() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    display.drawXbm(0, 0, Mesage_Logo_width, Mesage_Logo_height, Mesage_Logo_bits);
}
void draw_Mesage_text() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "Mesage");
}
//电话显示
void draw_Phone_image() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    display.drawXbm(0, 0, Phone_Logo_width, Phone_Logo_height, Phone_Logo_bits);
}
void draw_Phone_text() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "Phone");
}
//设置显示
void draw_Seting_image() {
    // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
    // on how to create xbm files
    display.drawXbm(0, 0, Seting_Logo_width, Seting_Logo_height, Seting_Logo_bits);
}
void draw_Seting_text() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "Seting");
}
void draw_Clock_Automatic(){
  Three_fun_flg = false;
  Clock_Automatic();
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 15, String("Weak:") + String(weekdays-1));
  display.drawString(64, 30, String(years) + ":" + String(months) + ":" + String(days));
  display.drawString(64, 45, String(hours) + ":" + String(minutes));
  display.display();
  delay(4000);
}
  
void draw_Clock_Manul(){
  Manul_Seting_flg = true;
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 0, String("Time Manul Seting"));
  if(Fourth_fun_flg){
    num = Number_str[Fourth_demoMode];
    if(Third_demoMode == 0) num1 = num;
    else if(Third_demoMode == 1) num2 = num;
    else if(Third_demoMode == 2) num3 = num;
    else if(Third_demoMode == 3) num4 = num;
    else if(Third_demoMode == 4) num5 = num;
    else if(Third_demoMode == 5) num6 = num;
    else if(Third_demoMode == 6) {
      Date_flg = true;
      Fourth_fun_flg = !Fourth_fun_flg;
      Manul_Seting_flg = !Manul_Seting_flg;
      minutes = num3*10 + num4;
      seconds = num5*10 + num6;
    }
  }
//  String Hours = (String(hours).length() == 1 ? String(num1)+String(" ")+String(hours) : String(" ") + String(hours));
//  String Minutes = (String(minutes).length() == 1 ? String(num2)+String(" ")+String(minutes) : String(" ") + String(minutes));
//  String Seconds = (String(seconds).length() == 1 ? String(num3)+String(" ")+String(seconds) : String(" ") + String(seconds));
  String Hours = String(num1)+String(" ")+String(num2);
  String Minutes = String(num3)+String(" ")+String(num4);
  String Seconds = String(num5)+String(" ")+String(num6);
  if(Hours.length() == 1) Hours = "0" + Hours;
  if(Minutes.length() == 1) Minutes = "0" + Minutes;
  if(Seconds.length() == 1) Seconds = "0" + Seconds;
  display.drawString(64, 15, Hours + " " + Minutes + " " + Seconds);
  if(Third_demoMode == 6) draw_Spot(64,45,true);
  else draw_Spot(44+Third_demoMode*9,25,true);
  display.drawString(64, 35, "OK");
  display.display();
}
void draw_DHT_temphumi(){
  Three_fun_flg = false;
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  if(temp+humi == tol) display.drawString(64, 32, String("Tmp:") + String(temp) + "    " + String("Hmi:") + String(humi));
  else display.drawString(64, 32, "Data Error"); 
  display.display();
  delay(2000);
}
void draw_IR_Receive(){
  Three_fun_flg = false;
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 32, String("IRremote") + (IR_flg == true ? String("   Opend") : String("   Closed"))); 
  display.display();
  delay(1000);
}
void Seting_Second_Fuc(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0,  "<1>Date manul Steing");
  display.drawString(0, 12,  "<2>Date automatic");
  display.drawString(0, 24,  "<3>DHT temp-humi");
  display.drawString(0, 36,  "<4>IR remote" );
  display.drawString(100,36,IR_flg == true ? String("ON") : String("OFF"));
  display.drawString(0, 48, "<5>Quit");  
  draw_Spot(128, Sec_demoMode*12, true);  
  if(Three_fun_flg){
    if(Sec_demoMode == 0) {
      draw_Clock_Manul(); 
    }
    else if(Sec_demoMode == 1) {
      Date_flg  = true;
      draw_Clock_Automatic();
    }
    else if(Sec_demoMode == 2){
      DHT_Read(true);
      draw_DHT_temphumi();
    }
    else if(Sec_demoMode == 3){
      IR_flg = !IR_flg;
      if(IR_flg) IR_init();
      draw_IR_Receive();
    }
    else if(Sec_demoMode == 4){
      Second_fun_flg = false;
      Three_fun_flg = false;
      Sec_demoMode = 0;
    }
  }
}
//音乐显示
void draw_Music_image(){
    display.drawXbm(0, 0, Music_Logo_width, Music_Logo_height, Music_Logo_bits);
}
void draw_Music_text() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "Music");
}
//位置显示
void draw_Location_image(){
    display.drawXbm(0, 0, Location_Logo_width, Location_Logo_height, Location_Logo_bits);
}
void draw_Location_text() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "Location");
}
//天气显示
void draw_Weather_image(){
    display.drawXbm(0, 0, Weather_Logo_width, Weather_Logo_height, Weather_Logo_bits);
}
void draw_Weather_text() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "Weather");
}
void draw_Weather(){
  Second_fun_flg = !Second_fun_flg;
  display.clear();
  if(Weather_draw_count == 1) display.drawXbm(0, 0, qin_width, qin_height, qin_bits);
  else if(Weather_draw_count == 2) display.drawXbm(0, 0, qin_duoyun_width, qin_duoyun_height, qin_duoyun_bits);
  else if(Weather_draw_count == 3) display.drawXbm(0, 0, duoyun_width, duoyun_height, duoyun_bits);
  else if(Weather_draw_count == 4) display.drawXbm(0, 0, baoyu_width, baoyu_height, baoyu_bits);
  else if(Weather_draw_count == 5) display.drawXbm(0, 0, feng_width, feng_height, feng_bits);
  else if(Weather_draw_count == 6) display.drawXbm(0, 0, xue_width, xue_height, xue_bits);
  else if(Weather_draw_count == 7) display.drawXbm(0, 0, yu_xue_width, yu_xue_height, yu_xue_bits);
  else if(Weather_draw_count == 8) display.drawXbm(0, 0, yu_width, yu_height, yu_bits);
//  display.setTextAlignment(TEXT_ALIGN_LEFT);
//  display.drawString(0, 0, String("Temp: ") + Weather_Temp);
  display.display();
  delay(3000);
}
void Weather_Second_Fuc(){
  Weather_info();
  draw_Weather();
}


//文件管理
void draw_FileMange_image(){
  display.drawXbm(0, 0, FileMange_Logo_width, FileMange_Logo_height, FileMange_Logo_bits);
}
void draw_FileMange_text() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(64, 0, "FileMange");
}

void draw_SD_Basic(){
  Three_fun_flg = false;
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, SD_Msg);
  display.drawString(64, 42, SD_Size);
  display.display();
  delay(2000);
}
void draw_listDir(){
    Manul_Seting_flg = true;
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    for(int i=0; i<Listdir_Num; i++){
      display.drawString(0, i*12,String(File_flg[i] == true ? "D-->" : "  F->") + Listdir_Info[i]);
    }
    display.drawString(0, Listdir_Num*12,String("<!>:Quit"));
    draw_Spot(128, Third_demoMode*12, true);
    if(Fourth_fun_flg) {
      if(Third_demoMode == Listdir_Num) Three_fun_flg = false , Fourth_fun_flg = false , Third_demoMode = 0;
      else ;
    }
    display.display();
}
void draw_fuckyou(){
  Three_fun_flg = false;
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(64,27,String("fuck"));
  display.display();
  delay(2000);
}
void FileMange_Second_Fuc(){
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0,  "<1>SD Basic");
  display.drawString(0, 12, "<2>SD List");
  display.drawString(0, 24, "<3>Quit");  
  draw_Spot(128, Sec_demoMode*12, true); 
  if(Three_fun_flg){
    if(Sec_demoMode == 0) {
      SD_Init();
      SD_Basic();
      if(SD_init) draw_SD_Basic();
      else {
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(64,27, SD_Msg);
      }
    }
    else if(Sec_demoMode == 1) {
      if(SD_flg) draw_listDir();
      else if(!SD_flg){
        SD_Init();
        Serial.println("SD inited ok");
        listDir(SD, "/", 0);
        Serial.println("SD listdir ok");
        if(SD_init) { 
          Serial.println("Read SD");
          for(int i=0; i < Listdir_Num; i++){
            Serial.println(Listdir_Info[i]);
          }
        }
      }
      else {
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(64,27, SD_Msg);
      }      
    }
    else if(Sec_demoMode == 2) {
       Sec_demoMode = 0;
       Second_fun_flg = false;
       Three_fun_flg = false;
    }
  }
}

//第二层的函数
void draw_Second_Fuc(){
  if(demoMode == 0) Wifi_Second_Fuc(); 
  if(demoMode == 1) Bluetooch_Second_Fuc();
  if(demoMode == 4) Seting_Second_Fuc();
  if(demoMode == 7) Weather_Second_Fuc();
  if(demoMode == 8) FileMange_Second_Fuc();
}
Demo demos[] = {draw_Wifi_image, draw_Bluetooch_image, draw_Mesage_image, draw_Phone_image, draw_Seting_image, draw_Music_image, draw_Location_image, draw_Weather_image,draw_FileMange_image};
Demo demos_text[] = {draw_Wifi_text, draw_Bluetooch_text, draw_Mesage_text, draw_Phone_text, draw_Seting_text, draw_Music_text, draw_Location_text, draw_Weather_text,draw_FileMange_text};

void Display(){
  if(!OLED_Pro_flg){
    // clear the display
    display.clear();
    draw_IR();
    if(!Second_fun_flg){
      // draw the current demo method
      demos[demoMode]();
      demos_text[demoMode]();
      fuck();
      msOverlay();
      //Serial.println(demoMode);
    }
    else draw_Second_Fuc();
    // write the buffer to the display
    display.display();
    delay(300);
  }
}

