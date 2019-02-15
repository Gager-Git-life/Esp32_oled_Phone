
//1
//信号强度：AT+CSQ
//命令返回：+CSQ: **,##
//其中**应在10到31之间，数值越大表明信号质量越好，##应为99。

//2
//修改串口波特率：AT+IPR
//命令格式：AT+IPR=19200

//3
//打电话：ATD
//命令格式：ATD**********;

//4
//挂机：ATH(电话已接通)
//挂机：AT+CHUP(电话未接通)


//短信
//1.选择短信存储载体：AT+CPMS
//AT+CPMS="SM" 返回：+CPMS: 8,15,8,15,1,40 OK
//SM（SIM卡）存储器总容量为15，当前存储量8；ME（模块）存储器总容量为40，当前存储量1；mem1定义为SM；

//2.设置短信格式：AT+CMGF=<mode>
//3.发送短信：AT+CMGS=13510090403
//4.读短信：AT+CMGR=<indes> 读第indes号短信

#include <SoftwareSerial.h>
//实例化软串口
SoftwareSerial mySerial(2, 3); // RX, TX

String Get_Msg = "";

void Signal_Detection(){
  Serial.println("AT+CSQ");
  while(Serial.available() > 0){
    Get_Msg = Serial.read();
  }
}




