#ifndef DISPLAY_H
#define DISPLAY_H
#include <U8g2lib.h>
#include "utilities.h"
#include <U8g2lib.h>
#include "Network.h"
class Display
{
private:

public:

  void init_love( U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2,Network wifi,int bounce,int dire,int nowPage);
  void wait_wifi();

};

#endif
