extern int Num,Listdir_Num;
extern int demoMode,Sec_demoMode,Third_demoMode,Fourth_demoMode,last_recode_time;
extern bool Second_fun_flg,Three_fun_flg,Fourth_fun_flg,Five_fun_flg,Manul_Seting_flg,WiFi_Manul_Link,SD_flg;

#define Esp32
//###############IO及串口初始化部分代码###############//
//按键引脚声明
int demoLength = 9;
#ifdef Esp8266
  const int Previous = D6; //前一个
  const int Latter = D5; //后一个
  const int Selected = D7; //选中
#endif

#ifdef Esp32
  const int Previous = 17; //前一个
  const int Latter = 14; //后一个
  const int Selected = 16; //选中
#endif

const int Second_fuc_count[] = {4,5,0,0,5,0,0,0,3};
const int Third_fuc_count[] =  {4,0,0,0,7,0,0,0,5};


void Pin_init(){
  Serial.println("IO Initeing ....");
  //LED
//  pinMode(16,OUTPUT);
//  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
//  delay(50); 
//  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high  
  //其它引脚
  pinMode(Previous, INPUT_PULLUP);
  pinMode(Latter, INPUT_PULLUP);
  pinMode(Selected, INPUT_PULLUP);
  //pinMode(REturn, INPUT);
  Serial.println("IO inited");
  delay(500);
}

void IR_datection(int count){
  if(count == 1){//第一层
    if(Num == 0) ;
    else if(Num == 1) Second_fun_flg = true , last_recode_time = millis();
    else if(Num == 2) {
      if(demoMode == 0) demoMode = (demoLength  - 1);
      else demoMode = ( demoMode - 1);
      last_recode_time = millis();
    }
    else if(Num == 3){
      demoMode = (demoMode + 1)%demoLength; 
      last_recode_time = millis();
    }
  }
  else if(count == 2){//第二层
    if(Num == 0) ;
    else if(Num == 1) Three_fun_flg = true, last_recode_time = millis();
    else if(Num == 2) {
      if(Sec_demoMode == 0) Sec_demoMode = Second_fuc_count[demoMode]-1;
      else Sec_demoMode -= 1;
      last_recode_time = millis();
    }
    else if(Num == 3) {
      Sec_demoMode = (Sec_demoMode+1)%Second_fuc_count[demoMode]; 
      last_recode_time = millis();  
    }
  }
  else if(count ==3){//第三层
    if(Manul_Seting_flg){
      if(Num == 0) ;
      if(Num == 1){
        Fourth_fun_flg = true;
        last_recode_time = millis();
      }
      else if(Num == 2){
        if(Fourth_fun_flg){
          Fourth_demoMode == 0 ? Fourth_demoMode = 9 : Fourth_demoMode -= 1;
        }
        else if(Third_demoMode == 0) Third_demoMode = (Third_fuc_count[demoMode]  - 1);
        else Third_demoMode = ( Third_demoMode - 1);
        last_recode_time = millis();
      }
      else if(Num ==3){
        if(Fourth_fun_flg){
          Fourth_demoMode == 9 ? Fourth_demoMode = 0 : Fourth_demoMode += 1;
        }
        else Third_demoMode = (Third_demoMode + 1)%Third_fuc_count[demoMode];
        last_recode_time = millis(); 
      }
    }
    else if(Num == 1){
      Fourth_fun_flg = true;
      last_recode_time = millis();
    }
  }
  Num = 0;
}

void IO_detection(){
  //读取io状态
  //第一层
  if(!Second_fun_flg){
    IR_datection(1);
    if(digitalRead(Previous) == LOW){
      //delay(50);
      if(digitalRead(Previous) == LOW){
         demoMode = (demoMode + 1)%demoLength; 
         last_recode_time = millis(); 
        }
        //delay(150);
    }
    else if(digitalRead(Latter) == LOW){
      //delay(50);
      if(digitalRead(Latter) == LOW){
        if(demoMode == 0) demoMode = (demoLength  - 1);
        else demoMode = ( demoMode - 1);
        last_recode_time = millis();
      }
      //delay(150);
    }
    else if(digitalRead(Selected) == LOW){
      //delay(50);
      if(digitalRead(Selected) == LOW){
          Second_fun_flg = true;
          last_recode_time = millis();
      }
      //delay(150);
    }
  }
  //第二层
  else if(Second_fun_flg && !Three_fun_flg){
    IR_datection(2);
    if(digitalRead(Previous) == LOW){
      //delay(50);  
      if(digitalRead(Previous) == LOW){
        Sec_demoMode = (Sec_demoMode+1)%Second_fuc_count[demoMode]; 
        last_recode_time = millis();
      }
      //delay(150);
    }
    else if(digitalRead(Latter) == LOW){
      //delay(50);
      if(digitalRead(Latter) == LOW){
        if(Sec_demoMode == 0) Sec_demoMode = Second_fuc_count[demoMode]-1;
        else Sec_demoMode -= 1;
        last_recode_time = millis();
      }
      //delay(150);
    }
    else if(digitalRead(Selected) == LOW){
      //delay(50);
      if(digitalRead(Selected) == LOW){
          Three_fun_flg = true;
          last_recode_time = millis();
      }
      //delay(150);
    }      
  }
  //第三层
  else if(Three_fun_flg ){
    IR_datection(3);
    if(Manul_Seting_flg){
      if(digitalRead(Previous) == LOW){
        //delay(50);
        if(digitalRead(Previous) == LOW){
           if(Fourth_fun_flg){
              Fourth_demoMode == 9 ? Fourth_demoMode = 0 : Fourth_demoMode += 1;
           }
           else Third_demoMode = (Third_demoMode + 1)%Third_fuc_count[demoMode];
           last_recode_time = millis(); 
          }
          //delay(150);
      }
      else if(digitalRead(Latter) == LOW){
        //delay(50);
        if(digitalRead(Latter) == LOW){
          if(Fourth_fun_flg){
              Fourth_demoMode == 0 ? Fourth_demoMode = 9 : Fourth_demoMode -= 1;
           }
          else if(Third_demoMode == 0) Third_demoMode = (Third_fuc_count[demoMode]  - 1);
          else Third_demoMode = ( Third_demoMode - 1);
          last_recode_time = millis();
        }
        //delay(150);
      }
      else if(digitalRead(Selected) == LOW){
        //delay(50);
        if(digitalRead(Selected) == LOW){
            Fourth_fun_flg = !Fourth_fun_flg;
            last_recode_time = millis();
        }
        //delay(150);
      }  
    }
    else if(digitalRead(Selected) == LOW){
      //delay(50);
      if(digitalRead(Selected) == LOW){
          Three_fun_flg = false;
          last_recode_time = millis();
      }
      //delay(150);
    }     
  }
  //第四层
  else if(Fourth_fun_flg){
    if(WiFi_Manul_Link){
      if(digitalRead(Previous) == LOW){
        //delay(50);
        if(digitalRead(Previous) == LOW){
           if(Five_fun_flg){
             if(Five_demoMode == 9) Five_demoMode = 0;
             else Five_demoMode += 1;
           }
           else Fourth_demoMode = (Fourth_demoMode + 1); 
           last_recode_time = millis(); 
        }
          //delay(150);
      }
      else if(digitalRead(Latter) == LOW){
        //delay(50);
        if(digitalRead(Latter) == LOW){
          if(Five_fun_flg){
            if(Five_demoMode == 0) Five_demoMode = 9;
            else Five_demoMode -= 1;
          }
          else if(Fourth_demoMode == 0) Fourth_demoMode = 0;
          else Fourth_demoMode = ( Fourth_demoMode - 1);
          last_recode_time = millis();
        }
        //delay(150);
      }
      else if(digitalRead(Selected) == LOW){
        //delay(50);
        if(digitalRead(Selected) == LOW){
            Five_fun_flg = !Five_fun_flg;
            last_recode_time = millis();
        }
        //delay(150);
      }  
    }
    else if(digitalRead(Selected) == LOW){
      //delay(50);
      if(digitalRead(Selected) == LOW){
          Fourth_fun_flg = false;
          last_recode_time = millis();
      }
      //delay(150);
    }   
  }
}
