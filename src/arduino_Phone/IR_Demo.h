#include "IRremote.h"

// 红外一体化接收头连接到Arduino 4号引脚   
#define RECV_PIN 4  

bool IR_flg = false;
bool IR_Receive_Flg = false;
unsigned long IR_receive = 0;

String IR_Num[] = {"c101e57b","9716be3f","3d9ae3f7","6182021b","8c22657b","488f3cbb","449e79f","32c6fdf7","1bc0157b","3ec3fc1b"};
String Fun_Num[] = {"dc238877","dc2308f7","dc2348b7"};

IRrecv irrecv(RECV_PIN);   
decode_results results; // 用于存储编码结果的对象 

void IR_init(){
  irrecv.enableIRIn(); // 初始化红外解码
}

void IR_Start(bool debug){
  if (irrecv.decode(&results)) {
    IR_Receive_Flg = true;
    IR_receive = results.value;
    if(debug){
      Serial.println(results.value, HEX);
      Serial.print("Receive:");
      Serial.println(String(IR_receive));
    }
    delay(500);
    irrecv.resume(); // 接收下一个编码
  }
  else IR_Receive_Flg = false;
}
