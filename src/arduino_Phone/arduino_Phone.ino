#include "Public.h"
#include "Displayer_Mode.h"
#include "Button_Mode.h"


int demoMode = 0;
int Sec_demoMode = 0;
int Third_demoMode = 0;
int Fourth_demoMode = 0;
int Five_demoMode = 0;
int last_recode_time = 0;


bool Second_fun_flg = false;
bool Three_fun_flg = false;
bool Fourth_fun_flg = false;
bool Five_fun_flg = false;
bool Return_flg = false;
bool OLED_Pro_flg = false;
bool Manul_Seting_flg = false;

char Number_str[] = {0,1,2,3,4,5,6,7,8,9};
char Letter_Small_str[] = {'a','b','c','d','e','f','g','h','i',
                     'j','k','l','m','n','o','p','q','r',
                     's','t','u','v','w','x','y','z'};
char Letter_Big_str[] = {'A','B','C','D','E','F','G','H','I',
                     'J','K','L','M','N','O','P','Q','R',
                     'S','T','U','V','W','X','Y','Z'};                            

void Serial_init(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("Serial init ok!");
  Serial.println();
}


void setup() {
  Serial_init();
  Pin_init();
  Display_init();
  draw_Start_image();
}
void loop() {
  OLED_protection();
  IO_detection();
  Display();
}
