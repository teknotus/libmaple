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

From the datasheet for the 25LC640 (64kbit) EEPROM:

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

Note: this library assumes that the program using the EEPROM has
exclusive access to the SPI bus. If that is not the case, the caller
has to use the begin() method to reinitialize the SPI hardware with
its speed, endianness, and mode

*/

#include "wirish.h"
#include "eeprom25xxx.h"
#include "EEPROM25xxx.h"

EEPROM25xxx::EEPROM25xxx(int newChipSelectPin, uint32 newSpiNum) {
    chipSelectPin = newChipSelectPin;
    spiNum = newSpiNum;
}

void EEPROM25xxx::begin(void) {
    eeprom_25xxx_begin(spiNum, chipSelectPin);
}

void EEPROM25xxx::write(uint16 address, uint8 data) {
    eeprom_25xxx_write(spiNum, chipSelectPin, address, data);
}

uint8 EEPROM25xxx::read(uint16 address) {
    return eeprom_25xxx_read(spiNum, chipSelectPin, address);
}

boolean EEPROM25xxx::writeInProgress(void) {
    return eeprom_25xxx_write_in_progress(spiNum, chipSelectPin);
}

boolean EEPROM25xxx::writeEnabled(void) {
    return eeprom_25xxx_write_enabled(spiNum, chipSelectPin);
}

void EEPROM25xxx::disable(void) {
    eeprom_25xxx_disable(chipSelectPin);
}

void EEPROM25xxx::enable(void) {
    eeprom_25xxx_enable(chipSelectPin);
}

