#include "Public.h"
#include<string.h>
#include <ArduinoJson.h>

#ifdef Esp8266
  SSD1306  display(0x3c, D2, D1);
#endif

#ifdef Esp32
  SSD1306  display(0x3c, 15, 0);
#endif

WiFiClient client;
const char* ssid = "TP-LINK_3ADB";
const char* password = "oyj961014";

//用于合成待请求url
const char* host = "api.thinkpage.cn";
const char* APIKEY = "gso5ckc10it60dy8";        //API KEY
const char* city = "loudi";
const char* language = "en";

const unsigned long HTTP_TIMEOUT = 2100;               // max respone time from server
const size_t MAX_CONTENT_SIZE = 2048;                   // max size of the HTTP response

void initWIFI(){
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
//    display.clear();
//    display.drawString(64, 10, "Connecting to WiFi");
//    display.drawXbm(46, 30, 8, 8, counter % 3 == 0 ? activeSymbole : inactiveSymbole);
//    display.drawXbm(60, 30, 8, 8, counter % 3 == 1 ? activeSymbole : inactiveSymbole);
//    display.drawXbm(74, 30, 8, 8, counter % 3 == 2 ? activeSymbole : inactiveSymbole);
//    display.display();
//
//    counter++;
  }
//  display.clear();
  Serial.println("");
  Serial.println("WiFi connected");
}

// 是否成功连接
bool connect(const char* hostName) {
  Serial.print("Connect to ");
  Serial.println(hostName);
  bool ok = client.connect(hostName, 80);
  Serial.println(ok ? "Connected" : "Connection Failed!");
  return ok;
}

//延时等待
void wait(int tm) {
  Serial.println("Wait.....");
  delay(tm*1000);
}

//  -- 跳过 HTTP 头，使我们在响应正文的开头
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";
 
  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);
 
  if (!ok) {
    Serial.println("No response or invalid response!");
  }
 
  return ok;
}
 
// 发送请求指令
bool sendRequest(const char* host, const char* cityid, const char* apiKey) {
  // 合成一个请求url
  //心知天气
  //心知天气获取连接：https://api.thinkpage.cn/v3/weather/daily.json?key=(KEY)&location=(city)&language=(language)&unit=c&start=-1&days=5
  String GetUrl = "/v3/weather/now.json?key=";
  GetUrl += apiKey;
  GetUrl += "&location=";
  GetUrl += city;
  GetUrl += "&language=";
  GetUrl += language;
 
  // 合成http请求发送到服务器
  client.print(String("GET ") + GetUrl + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  return true;
}
 
// 从HTTP服务器响应中读取正文
void readReponseContent(char* content, size_t maxSize) {
  size_t length = client.readBytes(content, maxSize);
  delay(100);
  Serial.println("Get the data from Internet!");
  content[length] = 0;
  Serial.println(content);
  Serial.println("Read Over!");
}

//  -- 关闭与HTTP服务器连接
void stopConnect() {
  Serial.println("Disconnect");
  client.stop();
}

 
//  我们要从此网页中提取的数据的类型
struct UserData {
  char city[16];
  char cnty[16];
  char weather[16];
  char code[16];
  char temp[16];
  char udate[16];
};
//UserData userData;

// 解析数据
bool parseUserData(char* content, struct UserData* userData) {
//  char json[] =
//      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
//  //  -- 根据我们需要解析的数据来计算JSON缓冲区最佳大小
//  //  -- 如果你使用StaticJsonBuffer时才需要
//  const size_t BUFFER_SIZE = MAX_CONTENT_SIZE;
// 
//  //  -- 在堆栈上分配一个临时内存池
//  StaticJsonBuffer<1024> jsonBuffer;
//  //  -- 如果堆栈的内存池太大，使用 DynamicJsonBuffer jsonBuffer 代替
//  // If the memory pool is too big for the stack, use this instead:
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(content);
  if (!root.success()) {
    Serial.println("JSON parsing failed!");   return false;
  }
  //  -- 复制我们感兴趣的字符串
  strcpy(userData->city, root["results"][0]["location"]["name"]);
  strcpy(userData->cnty, root["results"][0]["location"]["country"]);
  strcpy(userData->weather, root["results"][0]["now"]["text"]);
  strcpy(userData->code, root["results"][0]["now"]["code"]);
  strcpy(userData->temp, root["results"][0]["now"]["temperature"]);
  strcpy(userData->udate, root["results"][0]["last_update"]);
  //  -- 这不是强制复制，你可以使用指针，因为他们是指向“内容”缓冲区内，所以你需要确保
  //   当你读取字符串时它仍在内存
  return true;
}

void draw_printBuffer(const struct UserData* userData) {
  // Initialize the log buffer
  // allocate memory to store 8 lines of text and 30 chars per line.
  display.setLogBuffer(5, 30);
//  char updated[16] ;
  String City,Cnty,Weather,Code,Tmp,Updated;
      City = "city---->" + String(userData->city);
      Cnty = "county-->" + String(userData->cnty);
   Weather = "weather->" + String(userData->weather);
      Code = "code---->" + String(userData->code);
       Tmp = "tmp----->" + String(userData->temp);
//   Updated = String(userData->udate);
//   Updated.toCharArray(updated,10);
   Updated = "Update-->" + String(userData->udate);
  // Some test data
  String test[] = {
      City,
      Cnty,
      Weather,
      Code,
      Tmp,
      Updated,
     "end"
  };
  for (uint8_t i = 0; i < 11; i++) {
    display.clear();
    // Print to the screen
    display.println(test[i]);
    // Draw it to the internal screen buffer
    display.drawLogBuffer(0, 0);
    // Display it on the screen
    display.display();
    delay(500);
  }
}
int Weather_draw_count = 0;
String Weather_Temp = "";
bool Weather_draw_flg = false;

void Code_Detection(const struct UserData* userData){
  Weather_draw_flg = true;
  if(String(userData->code) == "0" || String(userData->code) == "1" || String(userData->code) == "2" || String(userData->code) == "3") Weather_draw_count = 1;
  if(String(userData->code) == "5" || String(userData->code) == "6" || String(userData->code) == "7" || String(userData->code) == "8") Weather_draw_count = 2;
  if(String(userData->code) == "4" || String(userData->code) == "9" || String(userData->code) == "26" || String(userData->code) == "28") Weather_draw_count = 3;
  if(String(userData->code) == "16" || String(userData->code) == "17" || String(userData->code) == "18") Weather_draw_count = 4;
  if(String(userData->code) == "32" || String(userData->code) == "33" ) Weather_draw_count = 5;
  if(String(userData->code) == "21" || String(userData->code) == "22" || String(userData->code) == "23" || String(userData->code) == "24" || String(userData->code) == "25") Weather_draw_count = 6;
  if(String(userData->code) == "19" || String(userData->code) == "20" ) Weather_draw_count = 7;
  if(String(userData->code) == "10" || String(userData->code) == "11" || String(userData->code) == "13" || String(userData->code) == "14" || String(userData->code) == "15") Weather_draw_count = 8;
  Weather_Temp = String(userData->temp);
}

 
// Print the data extracted from the JSON -- 打印从JSON中提取的数据
void printUserData(const struct UserData* userData) {
  Serial.println("Print parsed data :");
  Serial.print("City : ");
  Serial.print(userData->city);
  Serial.print(", \t");
  Serial.print("Country : ");
  Serial.println(userData->cnty);
   
  Serial.print("Weather天气 : ");
  Serial.print(userData->weather);
  Serial.print(",\t");
  Serial.print("Temp : ");
  Serial.print(userData->temp);
  Serial.print(" C");
  Serial.print(",\t");
  Serial.print("Last Updata : ");
  Serial.print(userData->udate);
  Serial.println("");
}

void Weather_info(){
  initWIFI();
  UserData userData;
  if (connect(host)) {
    if (sendRequest(host, city, APIKEY) && skipResponseHeaders()) {
    char response[MAX_CONTENT_SIZE];
    readReponseContent(response, sizeof(response));
    Serial.println("begian parse usedate");
    if (parseUserData(response, &userData)) {
      //printUserData(&userData);
      //draw_printBuffer(&userData); 
      Code_Detection(&userData); 
//      delay(100);
//      display.clear();
     }
    }
  }
  stopConnect();
  //wait(60);  
}





