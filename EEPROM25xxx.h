#ifndef EEPROM_25xxx_h
#define EEPROM_25xxx_h
/*

EEPROM_25xxx.cpp
Microchip 25xxx EEPROM Library for Maple 

Copyright (c) 2011 Adam Feuer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

See http://creativecommons.org/licenses/MIT/ for more information.

This library works with the following EEPROMs: 25xx640, 25xx128, 25xx512, 25xx1024

From the datasheet for the 25xx640 (64kbit) EEPROM:

* SPI interface
* Max. Clock 10 MHz
* 8192 x 8-bit Organization
* 32 Byte Page
* Self-Timed Erase and Write Cycles (5 ms max.)
* Block Write Protection
* Built-In Write Protection
* Sequential Read
* High Reliability
  * Endurance: 1,000,000 erase/write cycles
  * Data retention: > 200 years

http://datasheet.octopart.com/25LC640A-I/P-Microchip-datasheet-537224.pdf
  
*/

#include <wirish.h>

#define DEFAULT_CHIP_SELECT_PIN 10
#define DEFAULT_SPI_PORT 1
#define SPI_MODE_0 0
#define DUMMY_DATA 0xff

// EEPROM opcodes
#define WREN  6
#define WRDI  4
#define RDSR  5
#define WRSR  1
#define READ  3
#define WRITE 2

// EEPROM Status Register bitmasks
#define WIP_MASK 0x01
#define WEL_MASK 0x02

class EEPROM25xxx {
  private:
    int chipSelectPin;
    HardwareSPI *spiPtr;
    void sendAddress(uint16 address);
    boolean readStatusRegisterBit(uint8 mask);
  public:
    EEPROM25xxx(int, HardwareSPI *);
    void begin(void);
    void write(uint16, uint8);
    uint8 read(uint16);
    boolean writeInProgress(void);
    boolean writeEnabled(void);
    void disable(void);
    void enable(void);
};

#endif

