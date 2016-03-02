/*
  SLCD_HT1621 - Arduino library for 7seg 4digit/8digit LED matrix.
  Copyright 2016 morecat_lab
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#ifndef SLCD_HT1621_H
#define SLCD_HT1621_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "HT1621.h"

#define TS206 0
#define TS120 1
#define TS125 1
#define TS119 2
#define TS174 2

class SLCD_HT1621 {
private:
  uint8_t _data, _wr, _rd, _cs;
  uint8_t _model;
  HT1621 _ht = HT1621(0,0,0,0);
  uint8_t _supressZero = 1;
  struct lcdModelInfo {
    uint8_t large_num_digit;  // number of digit
    uint8_t large_num_start;  // start pos of number
    uint8_t small_num_digit;  // number of small digit
    uint8_t small_num_start;  // start pos of small number
    uint8_t pattern[17]; // pattern of "0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,-"
  } lcdInfo[3] = {
    // TS206
    {9, 6, 2, 2,
     {0x5F, 0x50, 0x3D, 0x79, 0x72, 0x6B, 0x6F, 0x51,
      0x7F, 0x7B, 0x77, 0x6E, 0x0F, 0x7C, 0x2F, 0x27, 0x20}},
    // TS120, TS125
    {8, 0, 2, 16,
     {0xEB, 0x60, 0xC7, 0xE5, 0x6C, 0xAD, 0xAF, 0xE0,
      0xEF, 0xED, 0xEE, 0x2F, 0x8B, 0x67, 0x8F, 0x8E, 0x04}},
    // TS119, TS174
    {7, 2, 1, 0,
     {0xAF, 0xA0, 0xCB, 0xE9, 0xE4, 0x6D, 0x6F, 0xA8,
      0xEF, 0xED, 0xEE, 0x67, 0x0F, 0xE3, 0x4F, 0x4E, 0x40}}
  };
  uint8_t _attrBuf[24];

public:
  SLCD_HT1621(uint8_t model, uint8_t data, uint8_t wr, uint8_t rd, uint8_t cs);
  void clear();
  void begin();
  void writeMem(uint8_t adr, uint8_t data);
  void writeMem8(uint8_t adr, uint8_t data);
  void setAttrBuf(uint8_t adr, uint8_t data);
  void setSupressZero(uint8_t flag);
  void setNum(uint8_t adr, uint8_t num);
  void clrCh(uint8_t adr);
  void printNum(long num);
  void printHex(long num);
  void printSNum(long num);
};

#endif

// EOF
