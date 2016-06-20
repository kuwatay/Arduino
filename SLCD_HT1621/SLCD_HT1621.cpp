/*
  SLCD_HT1621 - Arduino library for 7seg 4digit/8digit LED matrix.
  Copyright 2016 morecat_lab
 
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WConstants.h>
#endif
#include <pins_arduino.h>
#include "HT1621.h"
#include "SLCD_HT1621.h"

SLCD_HT1621::SLCD_HT1621(uint8_t model, uint8_t data, uint8_t wr, uint8_t rd, uint8_t cs) :
  _model(model), _data(data), _wr(wr), _rd(rd), _cs(cs) {
  _ht = HT1621(_data, _wr, _rd, _cs);
}

void SLCD_HT1621::setSupressZero(uint8_t f) {
  _supressZero = f;
}

void SLCD_HT1621::clear(void) {  // clear RAM
  for (uint8_t i = 0 ; i < 32 ; i++) {
    _ht.writeMem(i, 0x0);
  }
  for (uint8_t i = 0 ; i <sizeof _attrBuf ; i++) {
    _attrBuf[i] = 0;
  }
}

void SLCD_HT1621::begin(void) {
  _ht.begin();
  _ht.sendCommand(HT1621::BIAS_THIRD_4_COM);
  _ht.sendCommand(HT1621::SYS_EN);
  _ht.sendCommand(HT1621::LCD_ON);
  clear();
}

void SLCD_HT1621::writeMem(uint8_t adr, uint8_t data) {
  _ht.writeMem(adr, data &0xf);
}

void SLCD_HT1621::writeMem8(uint8_t adr, uint8_t data) {
  _ht.writeMem(adr, data & 0xf);
  _ht.writeMem(adr+1, data >> 4);
}

void SLCD_HT1621::setAttrBuf(uint8_t adr, uint8_t data) {
    _attrBuf[adr] = data & 0xf;
}

void SLCD_HT1621::setNum(uint8_t adr, uint8_t num) {
  _ht.writeMem(adr, (lcdInfo[_model].pattern[num] &0xf) | _attrBuf[adr]);
  _ht.writeMem(adr+1, (lcdInfo[_model].pattern[num] >> 4) | _attrBuf[adr+1]);
}

void SLCD_HT1621::clrCh(uint8_t adr) {
  _ht.writeMem(adr, _attrBuf[adr]);
  _ht.writeMem(adr+1, _attrBuf[adr+1]);
}

// print a number in large segment
void SLCD_HT1621::printNum(long num) {
  uint8_t minus;
  if (num < 0) { // minus
    num = - num;
    minus = 1;
  } else {
    minus = 0;
  }
  for (uint8_t i = 1 ; i <= lcdInfo[_model].large_num_digit ; i++) {
    uint8_t adr = (lcdInfo[_model].large_num_digit - i) * 2 + lcdInfo[_model].large_num_start;
    uint8_t n = num % 10;
    num /= 10;
    if (n == 0 && num == 0 && _supressZero == 1 && i != 1) {
      if (minus == 1) {
	setNum(adr, 16);	// '-'
	minus = 0;
      } else {
	clrCh(adr);
      }
    } else {
      setNum(adr, n);
    }
  }
}

// print a number in large segment in hex
void SLCD_HT1621::printHex(long num) {
  num = abs(num);
  for (uint8_t i = 1 ; i <= lcdInfo[_model].large_num_digit ; i++) {
    uint8_t adr = (lcdInfo[_model].large_num_digit - i) * 2 + lcdInfo[_model].large_num_start;
    uint8_t n = num % 16;
    num /= 16;
    if (n == 0 && num == 0 && _supressZero == 1) {
      clrCh(adr);
    } else {
      setNum(adr, n);
    }
  }
}

// print a number in small segment (if any)
void  SLCD_HT1621::printSNum(long num) {
  for (uint8_t i = 1 ; i <= lcdInfo[_model].small_num_digit ; i++) {
    int adr = (lcdInfo[_model].small_num_digit - i) * 2 +lcdInfo[_model].small_num_start;
    setNum(adr, num % 10);
    num /= 10;
  }
}

// EOF
