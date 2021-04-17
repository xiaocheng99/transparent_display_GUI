#include "network.h"
#include <Arduino_JSON.h>
#include <U8g2lib.h>
#include "utilities.h"

        
void Network::init(String ssid, String password)
{
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }

  Serial.println("");
  Serial.print("Connecting: ");
  Serial.print(ssid.c_str());
  Serial.print(" @");
  Serial.println(password.c_str());

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); //关闭STA模式下wifi休眠，提高响应速度
  WiFi.begin(ssid.c_str(), password.c_str());

      U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/ OLED_SCLK,
        /* data=*/  OLED_MOSI,
        /* cs=*/    OLED_CS,
        /* dc=*/    OLED_DC,
        /* reset=*/ OLED_RST);
        u8g2.begin();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  }

}

void Network::getTime()
{//获取时间
  HTTPClient http;
  http.begin("http://quan.suning.com/getSysTime.do");
  int httpCode=http.GET();
  if(httpCode>0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      String payload=http.getString();
      Serial.println(payload);
      JSONVar myObject = JSON.parse(payload);
       Serial.println( myObject["sysTime1"]);
       String timeData=payload.substring(46,payload.length() - 2);
       Serial.println( timeData);
        time_year=timeData.substring(0,4).toInt();
       Serial.println( time_year);
        time_month=timeData.substring(4,6).toInt();
       Serial.println( time_month);
        time_day=timeData.substring(6,8).toInt();
       Serial.println( time_day);
        time_hour=timeData.substring(8,10).toInt();
       Serial.println( time_hour);
        time_minute=timeData.substring(10,12).toInt();
       Serial.println( time_minute);
        time_second=timeData.substring(12,14).toInt();
       Serial.println( time_second);
       
      }
    }else{return ;}
    Serial.flush();
}

void Network::getWeather()
{
  HTTPClient http;
  http.begin("http://localhost/lovetv/");
  int httpCode=http.GET();
  if(httpCode>0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      String payload=http.getString();
      Serial.println(payload);
       Serial.flush();
      }
    }else{return ;}

}

String Network::getUdpMsg()
{

  int packetSize = udp.parsePacket();//获得解析包
  if (packetSize)//解析包不为空
  {
    //收到Udp数据包
    //Udp.remoteIP().toString().c_str()用于将获取的远端IP地址转化为字符串
    Serial.printf("收到来自远程IP：%s（远程端口：%d）的数据包字节数：%d\n", udp.remoteIP().toString().c_str(), udp.remotePort(), packetSize);
      
    // 解析UDP数据包中的所以数据，以字符串格式返回
    String udpStringVal = udp.readString(); 
    
    // 然后向串口打印返回的字符串
    Serial.print("开发板接收到UDP数据中的字符串 "); Serial.println(udpStringVal);
    return udpStringVal;
  }
   }



String Network::getTcpMsg()
{
   if(tcp.connected()){
    if (tcp.available()) //如果有数据可读取
            {
                String line = tcp.readStringUntil('\n'); //读取数据到换行符
                Serial.print("读取到数据：");
                Serial.println(line);
                tcp.write("replay"); //将收到的数据回发
                return line;
            }
}else if(!tcp.connected()){
      Serial.println("尝试访问服务器");
      tcp.connect(serverIP, serverPort);
      delay(2000);
      if (tcp.connected()) //尝试访问目标地址
    {
        Serial.println("访问成功");
    }  else
    {
        Serial.println("访问失败");
        
    }
      
      } 
   }

 
   







  
