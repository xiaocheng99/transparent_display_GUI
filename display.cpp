#include "display.h"
#include "utilities.h"
//#include "absDay.h"

void Display::init_love(U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2,Network wifi,int bounce,int dire,int nowPage)
{
  //初始化左边模块

    //画左边爱心
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawFrame(0,0,125,62);
    u8g2.drawLine(33,0,33,61);
    u8g2.drawLine(33,25,124,25);
    u8g2.setCursor(5, 15);
    u8g2.print("YJW");
    
    if(dire==0){
      u8g2.drawUTF8(10, 40+bounce, "☃");
    }else {
      u8g2.drawUTF8(10, 40-bounce, "☃");
      }
    
//    u8g2.drawXBM(10,25,heart_width,heart_height, heart_bits);
    u8g2.setCursor(5, 60);
   u8g2.print("LCC");
//画时间
    u8g2.setCursor(45, 18);
    if(wifi.time_hour<10)
    {
      String hour="0"+(String)wifi.time_hour;
      u8g2.print(hour);
     }else{
    u8g2.print(wifi.time_hour);}

    u8g2.setCursor(60, 18);
    u8g2.print(":");

   
    u8g2.setCursor(70, 18);
     if(wifi.time_minute<10)
    {
      String minute="0"+(String)wifi.time_minute;
      u8g2.print(minute);
     }else {
    u8g2.print(wifi.time_minute);
    }
    
    u8g2.setCursor(85, 18);
    u8g2.print(":");
    
    u8g2.setCursor(95, 18);
     if(wifi.time_second<10)
    {
      String second="0"+(String)wifi.time_second;
      u8g2.print(second);
     }else{
    u8g2.print(wifi.time_second);}
    //画日期差
    int numDayDis= 745;
    u8g2.setCursor(40,40);
//    String dayStr=(String)wifi.time_year+"/"+(String)wifi.time_month+"/"+(String)wifi.time_day;
    u8g2.print("love for");

    u8g2.setCursor(40,56);
   String dayStr=(String)numDayDis+" "+" days";
    u8g2.print(dayStr);
    
    if(dire==0) {dire=1;}
    else {dire=0;}
    delay(1000);
    wifi.time_second+=1;
    if( wifi.time_second>=60)
    {
      wifi.time_second=0;
      wifi.time_minute+=1;
      wifi.getTime();
      if(wifi.time_minute>=60)
      {
        wifi.time_minute=0;
        wifi.time_hour+=1;
        
        if(wifi.time_hour>=24)
        wifi.time_hour=0;
        }  
      }

  }

  void Display::wait_wifi()
  {
 U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/ OLED_SCLK,
        /* data=*/  OLED_MOSI,
        /* cs=*/    OLED_CS,
        /* dc=*/    OLED_DC,
        /* reset=*/ OLED_RST);
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.clearBuffer();
    u8g2.setDrawColor(1);
//    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
//    u8g2.drawXBM(0,0,logo_width,logo_height,logo_bits);
    u8g2.setFont(u8g2_font_luRS08_tf);
    u8g2.drawUTF8(10,30,"Connetcing My Love");
    u8g2.sendBuffer();
    }
