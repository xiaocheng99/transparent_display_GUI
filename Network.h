#ifndef NETWORK_H
#define NETWORK_H
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include "network.h"

class Network
{
private:


public:

  WiFiUDP udp;
  WiFiClient tcp;
//TCP使用
  const char * serverIP="42.192.164.131"; //欲访问的地址
  uint16_t serverPort = 5210; 
  //Udp使用 弃用
  const char * udpServerAddr = "0.0.0.0";
  const int udpServerPort = 1520;
  const int udpLocalPort = 5200;


  
  int time_year;
  int time_month;
  int time_day;
  int time_hour;
  int time_minute;
  int time_second;
  void init(String ssid, String password);
  void getTime();
  void getWeather();
  String getUdpMsg();
  String getTcpMsg();

};

#endif
