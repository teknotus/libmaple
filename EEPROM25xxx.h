/*

EEPROM25xxx.h
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
  
*/

/**
 * @file EEPROM25xxx.h
 * @brief Microchip 25xxx SPI EEPROM driver class declaration
 */

#ifndef EEPROM25xxx_h
#define EEPROM25xxx_h

#include "wirish.h"

class EEPROM25xxx {
  private:
    int chipSelectPin;
    uint32 spiNum;
  public:
    EEPROM25xxx(int, uint32);
    void begin(void);
    void write(uint16, uint8);
    uint8 read(uint16);
    boolean writeInProgress(void);
    boolean writeEnabled(void);
    void disable(void);
    void enable(void);
};

#endif

