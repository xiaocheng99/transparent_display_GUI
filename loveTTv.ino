#include <ETH.h>
#include <WiFi.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <IRremote.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <U8g2lib.h>
#include <pcf8563.h>
#include "utilities.h"
#define heart_width 10
#define heart_height 15

#include "display.h"
#include "network.h"
#include "absDay.h"
// INCLUDE CHRONO LIBRARY : http://github.com/SofaPirate/Chrono
#include <Chrono.h> 

//初始化主要类对象
Display screen;
Network wifi;
U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/ OLED_SCLK,
        /* data=*/  OLED_MOSI,
        /* cs=*/    OLED_CS,
        /* dc=*/    OLED_DC,
        /* reset=*/ OLED_RST);

//WIfiudp线程
Chrono udpChrono; 
Chrono timeChrono;
Chrono clientChrono;
//暂时无用处
IRrecv irrecv(IR_RECV);
PCF8563_Class rtc;
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp(&Wire1);
bool findAHT = false, findBMP = false, findRTC = false;
uint32_t timeStamp;

//配置信息
String  ssid  = "MERCURY_EBEE";
String  password  = "zxc528zxc";
const char *ntpServer           = "pool.ntp.org";
//const long  gmtOffset_sec       = 28800;
//const int   daylightOffset_sec  = 0;

//雪人跳跃幅度
int bounce=5;
//判断跳跃方向 0下 1上
int dire=0;
//当前所在页面编号 初始化为
int nowPage=PAGE_LOVE_TIME;


void setup() {
Serial.begin(115200);
    /*Turn on power control*/
    pinMode(PWR_ON, OUTPUT);
    digitalWrite(PWR_ON, HIGH);
    
    /*Power on the display*/
    pinMode(OLED_PWR, OUTPUT);
    digitalWrite(OLED_PWR, HIGH);

    /*Set touch Pin as input*/
    pinMode(TOUCH_PIN, INPUT);

    /*Touch chip power control*/
    pinMode(TOUCH_PWR, PULLUP);
    digitalWrite(TOUCH_PWR, HIGH);
    
    
    screen.wait_wifi();
    wifi.init(ssid,password);
    wifi.getTime();
    u8g2.enableUTF8Print();

}

void loop() {   

  if(WiFi.status() != WL_CONNECTED)
  {
    wifi.init(ssid,password);
   }
//类线程 30ms运行一次
  if (timeChrono.hasPassed(10) ) { // elapsed(1000) returns 1 if 1000ms have passed.
    timeChrono.restart();  // restart the Chrono 
   String msgVal= wifi.getTcpMsg();
   if(msgVal.toInt()!=0){ 
    String subMsgVal=msgVal.substring(msgVal.length()-1);
   nowPage=subMsgVal.toInt();
   Serial.println(nowPage);
}
//重连机制 
  if (clientChrono.hasPassed(180000) ) { 
    clientChrono.restart();  
     //每3分钟和服务器重连一下
    wifi.tcp.connect(wifi.serverIP, wifi.serverPort);
}
  }
//计算时间线程
 if (udpChrono.hasPassed(1000) ) { // elapsed(1000) returns 1 if 1000ms have passed.
    udpChrono.restart();

    if(dire==0) {dire=1;}
    else {dire=0;}
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


////页面1  
if(nowPage==PAGE_LOVE_TIME) {                        
  u8g2.firstPage();
  do{
   
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
    int numDayDis= dayDis(2019, 1, 16, wifi.time_year, wifi.time_month, wifi.time_day);
    u8g2.setCursor(40,40);
//    String dayStr=(String)wifi.time_year+"/"+(String)wifi.time_month+"/"+(String)wifi.time_day;
    u8g2.print("love for");

    u8g2.setCursor(40,56);
   String dayStr=(String)numDayDis+" "+" days";
    u8g2.print(dayStr);
    
    }while(u8g2.nextPage());

}else if(nowPage==PAGE_WEATHER)
{
  u8g2.firstPage();
  do{
    u8g2.setDrawColor(1);

    u8g2.setCursor(40, 40);
     u8g2.print("Page2");
    }while(u8g2.nextPage());
    
  }
}    







 
