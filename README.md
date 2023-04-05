
*作者：小成Charles
商业工作，学习交流请添加Vx：Lcc-Triumph
原创作品
转载请标注原创文章地址：[https://blog.csdn.net/weixin_42999453/article/details/113502220](https://blog.csdn.net/weixin_42999453/article/details/113502220)
github完整代码地址：[https://github.com/xiaocheng99/transparent_display_GUI](https://github.com/xiaocheng99/transparent_display_GUI)
*

## 一、前言
因为看了B站上稚晖君的嵌入式开发视频，搞得我这个软件开发的也想弄一弄嵌入式，然后就设计了一下，做了我这个第一个物联网项目，我称之为`LoveTV`,这个是做了送给女朋友的，为了方便控制它以及传输数据，这里想到就直接用网页传输数据，这样很方便！服务器是要搭建在我的`腾讯云`服务器，也就是理论上来说，可以在任何地方实现数据传输。话不多说，上项目图！
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210201115303669.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjk5OTQ1Mw==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210201110705547.GIF)
## 二、设计思路
上面只是雏形，后期会添加和美化更多功能！这里我实现网页控制换页的方法是网页通过点击按钮会执行相应的`PHP`文件，php后端会创建一个`udp`协议，然后将数据发送给用`C++`创建的`Udp服务端`，服务端收到消息后会把消息再转发给单片机创建的`Udp客户端`，单片机客户端收到数据后会做相应的处理。这就是大致的思路，为了方便理解，我大致的流程画个图。![在这里插入图片描述](https://img-blog.csdnimg.cn/20210201112113833.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80Mjk5OTQ1Mw==,size_16,color_FFFFFF,t_70)
## 三、核心代码(非完整代码)
（1）php后端
这里就是创建socket，指定为`udpsocket`,然后直接将数据传输给指定的服务器`IP`和`端口`，注意php使用socket要到php.ini文件里面把`extension=sockets`前面的分号去掉，这样才能使用socket。
```cpp
<?php
$sock = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
$msg = '1';
$len = strlen($msg);
socket_sendto($sock, $msg, $len, 0, serverIp,serverPort);
header("Location: http://lovetv/index.html");
socket_close($sock);
?>

```
（2）c++后端
这里主要就是实现数据的转发，基于C++的框架Qt写得
tvServer.h
```cpp
#ifndef TVSERVER_H
#define TVSERVER_H

#include <QUdpSocket>
#include <QObject>
#include <QList>

class tvServer : public QObject
{
    Q_OBJECT
public:
    explicit tvServer(QObject *parent = nullptr);
    QUdpSocket *udpServer;
    void bindServer(quint16 port);
    quint16 port;

    QHostAddress tvAddr;
    quint16 tvPort;

    QHostAddress webAddr;
    quint16 webPort;

    bool enter;

signals:

public slots:
    void onReadyRead();
};

#endif // TVSERVER_H

```
tvServer.cpp
```cpp
#include "tvserver.h"

tvServer::tvServer(QObject *parent) : QObject(parent)
{
    udpServer=new QUdpSocket ();
    //test ip
    tvAddr.setAddress("192.168.1.91");
    tvPort=1520;
    enter=false;
    bindServer(1520);
    connect(udpServer,&QUdpSocket::readyRead,this,&tvServer::onReadyRead);
}
void tvServer::bindServer(quint16 port)
{//bind value
//    QHostAddress addr;
//    addr.setAddress("192.168.1.110");

    if(udpServer->bind(port))
    {
        qDebug()<<"successful bind!";
    }else {
        qDebug()<<"falied to bind!";

    }



}
void tvServer::onReadyRead()
{//server to revecive
    QByteArray   datagram;
    datagram.resize(udpServer->pendingDatagramSize());
    QHostAddress    peerAddr;
    quint16 peerPort;
    udpServer->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);
    if(QString(datagram.data()) =="tv"&&enter==false)
    {
       qDebug()<<datagram.data()<<peerAddr<<peerPort;
       tvAddr=peerAddr;
       tvPort=peerPort;
       udpServer->writeDatagram("replay tv",peerAddr,peerPort);
       enter=true;
    }
    if(QString(datagram.data()) =="web"){
        webAddr=peerAddr;
        webPort=peerPort;
        qDebug()<<datagram.data()<<peerAddr<<peerPort;
}
//send msg to tv;

    qDebug()<<datagram.data()<<peerAddr<<peerPort;
    udpServer->writeDatagram(datagram.data(),tvAddr,tvPort);

}

```
（3）嵌入式c后端
编程IDE用的是`Arduino`，然后主板用的是`ESp32`,屏幕就是透明的`OLED`屏幕，前端不多说了，主要是利用`u8g2`这个图形框架写得，后端先连接`wifi`，然后利用`WiFiUDP`这个库创建客户端，一直循环去监听消息，收到消息后做出相应的判断；

这里是开启udp
```cpp
  //开启udp工具
    if(udp.begin(WiFi.localIP(),udpLocalPort))
    {
      Serial.printf("现在收听IP：%s, UDP端口：%d\n", WiFi.localIP().toString().c_str(), udpLocalPort);
      //将wifi信息传输给服务器
      udp.beginPacket(udpServerAddr, udpServerPort);//配置远端ip地址和端口
      udp.print("tv");//把数据写入发送缓冲区
      udp.endPacket();//发送数据
     }else{
      Serial.println("监听失败");
      }
```
getUdpMsg()这里就是一直获得解析包，可以在loop里面去一直调用这个函数获得返回数据。
```cpp
String Network::getUdpMsg()
{
//    udp.beginPacket(udpServerAddr, udpServerPort);//配置远端ip地址和端口
//    udp.print("mesg!");//把数据写入发送缓冲区
//    udp.endPacket();//发送数据

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
```
这里用了一个`Chrono`线程库，让这个函数一直处于一个单独的线程运行
`hasPassed()`里面给的参数越小，延迟就越小，单位为毫秒
```cpp
  if (timeChrono.hasPassed(10) ) { // elapsed(1000) returns 1 if 1000ms have passed.
    timeChrono.restart();  // restart the Chrono 
   String msgVal= wifi.getUdpMsg();
   if(msgVal.toInt()!=0){ 
   nowPage=msgVal.toInt();
   Serial.println(nowPage);
}
```
## 四、总结和扩展
目前还处于雏形阶段，就是理论已经成型了，接下来就是完善和优化了，需要改进的就是Udp协议虽然面型无连接但是由于单片机的性能太低，网络信号差，特别容易丢包，导致数据接收不到，那么改进就是换成`TCP`协议或者对`UDP`协议做一个类似于`TCp`三次握手的数据是否接受的检测来保证数据传输到了。

*作者：小成Charles
原创作品
转载请标注原创文章地址：[https://blog.csdn.net/weixin_42999453/article/details/113502220]
(https://blog.csdn.net/weixin_42999453/article/details/113502220)*


*商业工作，学习交流请添加Vx：Lcc-Triumph*
