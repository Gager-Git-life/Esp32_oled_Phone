#include "Public.h"


IPAddress local_IP(192, 168, 4, 4);  
IPAddress gateway(192, 168, 4, 1);  
IPAddress subnet(255, 255, 255, 0);  

int Network_count ;
const char* AP_SSID = "Gager";
const char* AP_PWD = "961014";
String Ssid[] = {};


bool WiFi_WPA_Flg = false;

bool AP_flg = false;

void Start_AP(){
  // put your setup code here, to run once:  
  WiFi.mode(WIFI_AP);
  //WiFi.softAPConfig(local_IP, gateway, subnet);  
  WiFi.softAP(AP_SSID);  
  Serial.print("Soft-AP IP address = ");  
  Serial.println(WiFi.softAPIP()); 
}

void Scan_Network(){
    //wifi init
    Serial.println("WIFI STA Seting....");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("WIFI Set ok");
    //wifi scan
    Serial.println("scan start");
    Network_count = WiFi.scanNetworks();
    Serial.println("scan done");  
}

void Link_AP(const char* ssid, const char* pwd){
  WiFiClient client;
  Serial.print("Connecting to ");
  Serial.println(ssid);
  if(WiFi_WPA_Flg) WiFi.begin(ssid, pwd); 
  else  WiFi.begin(ssid);
}

