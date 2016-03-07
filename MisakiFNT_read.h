/*
  MisakiFNT_read.h - for ESP-WROOM-02 ( esp8266 )
  Beta version 1.0
  This is the Arduino library for reading Misaki font. (For ESP8266) 
  
The MIT License (MIT)

Copyright (c) 2016 Mgo-tec
Blog URL ---> https://www.mgo-tec.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MisakiFNT_read_h_
#define MisakiFNT_read_h_
#include "Arduino.h"
#include "FS.h"

class MisakiFNT_read
{
public:
  MisakiFNT_read();
  uint8_t Sjis_To_MisakiFNT_DotRead(const char* _FNT_file_8x8, const char* _FNT_file_4x8, uint8_t __Direction, int16_t __Angle, uint8_t __SjisH, uint8_t __SjisL, uint8_t* __buf);
  void Sjis_To_Misaki_Font_Adrs(const char* _font_file, uint8_t __jisH, uint8_t __jisL, uint8_t* __buf);
  void SPIFFS_Flash_MisakiFontRead(const char* _font_file, uint16_t __addrs, uint8_t* __buf);
private:
};

#endif