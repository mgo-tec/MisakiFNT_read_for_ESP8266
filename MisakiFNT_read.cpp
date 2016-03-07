/*
  MisakiFNT_read.cpp - for ESP-WROOM-02 ( esp8266 )
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

#include "Arduino.h"
#include "MisakiFNT_read.h"

MisakiFNT_read::MisakiFNT_read(){}

//*******************美咲フォントメインクラス*************************************************************
uint8_t MisakiFNT_read::Sjis_To_MisakiFNT_DotRead(const char* _FNT_file_8x8, const char* _FNT_file_4x8, uint8_t __Direction, int16_t __Angle, uint8_t __SjisH, uint8_t __SjisL, uint8_t* __buf)
{
  uint16_t _fnt_adrs_half;
  uint8_t __cp;
  
  if((__SjisH>=0x20 && __SjisH<=0x7F) || (__SjisH>=0xA1 && __SjisH<=0xDF)){
  
    _fnt_adrs_half = 0x110 + (__SjisH - 0x20)*8;
    MisakiFNT_read::SPIFFS_Flash_MisakiFontRead(_FNT_file_4x8, _fnt_adrs_half, __buf);
    
    __cp = 1;
    
    if((__SjisL>=0x20 && __SjisL<=0x7F) || (__SjisL>=0xA1 && __SjisL<=0xDF)){
      _fnt_adrs_half = 0x110 + (__SjisL - 0x20)*8;
      uint8_t __dummy_buf[8];
      uint8_t i;
      
      MisakiFNT_read::SPIFFS_Flash_MisakiFontRead(_FNT_file_4x8, _fnt_adrs_half, __dummy_buf);
      switch(__Angle){
        case 0:
          switch(__Direction){
            default:
              for(i=0; i<8; i++){
                __dummy_buf[i] = __dummy_buf[i]>>4;
                __buf[i] = __buf[i] | __dummy_buf[i];
              }
              break;
            case 1:
              for(i=0; i<8; i++){
                __buf[i] = __buf[i]>>4;
                __buf[i] = __dummy_buf[i] | __buf[i];
              }
              break;
          }
          break;
        case 90:
          switch(__Direction){
            default:
              for(i=0; i<8; i++){
                __dummy_buf[i] = __dummy_buf[i]>>4;
                __buf[i] = __buf[i] | __dummy_buf[i];
              }
              break;
            case 2:
              for(i=0; i<8; i++){
                __buf[i] = __buf[i]>>4;
                __buf[i] = __dummy_buf[i] | __buf[i];
              }
              break;
          }
          break;
        case -90:
          switch(__Direction){
            default:
              for(i=0; i<8; i++){
                __dummy_buf[i] = __dummy_buf[i]>>4;
                __buf[i] = __buf[i] | __dummy_buf[i];
              }
              break;
            case 3:
              for(i=0; i<8; i++){
                __buf[i] = __buf[i]>>4;
                __buf[i] = __dummy_buf[i] | __buf[i];
              }
              break;
          }
          break;
        case 180:
          switch(__Direction){
            default:
              for(i=0; i<8; i++){
                __dummy_buf[i] = __dummy_buf[i]>>4;
                __buf[i] = __buf[i] | __dummy_buf[i];
              }
              break;
            case 0:
              for(i=0; i<8; i++){
                __buf[i] = __buf[i]>>4;
                __buf[i] = __dummy_buf[i] | __buf[i];
              }
              break;
          }
          break;
      }
      __cp = 2;
    }
  }else{
    uint8_t __dummy_buf2[8];
    MisakiFNT_read::Sjis_To_Misaki_Font_Adrs(_FNT_file_8x8, __SjisH, __SjisL, __dummy_buf2);
    for( byte _col = 0; _col < 8; _col++ ) {
      for( byte _row = 0; _row < 8; _row++ ) {
        bitWrite( __buf[7-_row], 7-_col , bitRead( __dummy_buf2[_col], 7-_row ) );
      }
    }
    __cp = 2;
  }
  return __cp;
}
//*******************Shift_JISコードから美咲フォントアドレス計算********************************************
void MisakiFNT_read::Sjis_To_Misaki_Font_Adrs(const char* _font_file, uint8_t __jisH, uint8_t __jisL, uint8_t* __buf) 
{    // S-JISコードからMisakiフォントファイル上のバイト位置をポインタで返す。
  uint16_t __SjisCode;
  int16_t __adj;
  uint16_t __fnt_adrs;

  if( __jisH != '\0'){  //'\0'ならば読み込まない。
    if((__jisH >= 0x81 && __jisH <=0x9f) || (__jisH >= 0xe0 && __jisH <=0xef)){  //全角の場合
      __SjisCode = ((uint16_t)__jisH << 8 )+__jisL;
      if(__SjisCode>=0x8140 && __SjisCode <=0x88fc){
        if     (__SjisCode>=0x8140 && __SjisCode<=0x817e) __adj =  16;  // 一般記号
        else if(__SjisCode>=0x8180 && __SjisCode<=0x81fc) __adj =  15;  // 一般記号
        else if(__SjisCode>=0x824f && __SjisCode<=0x8279) __adj = -52;  // 数字、英語大文字
        else if(__SjisCode>=0x8281 && __SjisCode<=0x82f1) __adj = -53;  // 英小文字、ひらがな
        else if(__SjisCode>=0x8340 && __SjisCode<=0x837e) __adj = -120;  // カタカナ
        else if(__SjisCode>=0x8380 && __SjisCode<=0x83d6) __adj = -121;  // カタカナ
        else if(__SjisCode>=0x8440 && __SjisCode<=0x847e) __adj = -188;  // 外国大文字
        else if(__SjisCode>=0x8480 && __SjisCode<=0x84be) __adj = -189;  // 外国小文字、罫線、※丸囲み文字やその他特殊文字は美咲フォントには無い。
        else if(__SjisCode>=0x8740 && __SjisCode<=0x877e) __adj = -768; // １３区
        else if(__SjisCode>=0x8780 && __SjisCode<=0x879c) __adj = -769; // １３区
        else if(__SjisCode>=0x889f && __SjisCode<=0x88fc) __adj = -837;  // 第一水準漢字 亜～蔭まで
        __fnt_adrs = ((__SjisCode-0x8140)+__adj)*8;
      }else if(__SjisCode>=0x8940 && __SjisCode<=0x9ffc){ //院～滌
        if(__jisL <= 0x7e){
          __fnt_adrs = ((__SjisCode-0x8140)-(836+(__jisH-0x88))-67*(__jisH-0x88))*8;
        }else if(__jisL >= 0x80 ){
          __fnt_adrs = ((__SjisCode-0x8140)-(837+(__jisH-0x88))-67*(__jisH-0x88))*8;
        }
      }else if(__SjisCode>=0xe040 && __SjisCode <=0xeaa4){ //漾～熙
        if(__jisL <= 0x7e ){
          __fnt_adrs = ((__SjisCode-0x8140)-(836+(__jisH-0x88))-67*(__jisH-0x88)-12032)*8;
        }else if(__jisL >= 0x80 ){
          __fnt_adrs = ((__SjisCode-0x8140)-(837+(__jisH-0x88))-67*(__jisH-0x88)-12032)*8;
        }
      }else{
        __fnt_adrs = (0x8140 + 16)*8;  // 対応文字コードがなければ 全角スペースを返す
      }
    }else {
      __fnt_adrs = (0x8140 + 16)*8;  // 対応文字コードがなければ 全角スペースを返す
    }
  }else {
    __fnt_adrs = (0x8140 + 16)*8;  // 対応文字コードがなければ 全角スペースを返す
  }
  
  MisakiFNT_read::SPIFFS_Flash_MisakiFontRead(_font_file, __fnt_adrs, __buf);
}

//*****************フォントファイル読み込み**************************************
void MisakiFNT_read::SPIFFS_Flash_MisakiFontRead(const char* _font_file, uint16_t __addrs, uint8_t* __buf)
{
//  Dir dir = SPIFFS.openDir("/");//これは読み取り速度が遅くなるかもしれないのでコメントアウトしている
  File __f1 = SPIFFS.open(_font_file, "r"); //美咲フォントファイル
  if(__f1){
    __f1.seek(__addrs,SeekSet);
    for (byte i=0; i<8; i++){
      __buf[i] = __f1.read(); //readBytes はSPIFFSファイルシステムver0.2.0ではchar型となるのでここでは使わない
    }
    __f1.close();
  }else{
    Serial.print(_font_file);
    Serial.println(F(" file has not been uploaded to the flash in SPIFFS file system"));
    delay(30000);
  }
}
