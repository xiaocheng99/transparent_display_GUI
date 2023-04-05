#!/bin/sh
exe="udpserver"
des="/opt/QTcode/build-udpserver-Desktop_Qt_5_12_3_GCC_64bit-Release/server1.0"
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')    
cp $deplist $des 
