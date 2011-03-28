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
has to reinitialize the SPI hardware with its speed, endianness, and
mode.

*/

#include "wirish.h"
#include "EEPROM25xxx.h"

EEPROM25xxx::EEPROM25xxx(int newChipSelectPin, HardwareSPI *newSpi) {
    chipSelectPin = newChipSelectPin;
    spiPtr = newSpi;
}

void EEPROM25xxx::begin(void) {
    pinMode(chipSelectPin, OUTPUT);
    eepromDisable();
    spiPtr->begin(SPI_9MHZ, MSBFIRST, SPI_MODE_0);
}

// write one byte
void EEPROM25xxx::write(uint16 address, uint8 data) {
  eepromEnable();
  spiPtr->send(WREN); 
  eepromDisable(); // Needed for EEPROM to accept write enable (WREN) opcode
  eepromEnable();
  spiPtr->send(WRITE); 
  eepromSendAddress(address);
  spiPtr->send(data); 
  eepromDisable(); // Needed for EEPROM to initiate write cycle
  eepromEnable();
  while (eepromWriteInProgress()) {
    // do nothing - busy wait, otherwise we get write errors
  }
}

// read one byte
uint8 EEPROM25xxx::read(uint16 address) {
  uint8 data;
  eepromEnable();
  spiPtr->send(READ); 
  eepromSendAddress(address);
  data = spiPtr->send(DUMMY_DATA);
  eepromDisable();
  return data;
}

void EEPROM25xxx::eepromSendAddress(uint16 address) {
  uint8 most_significant_byte = address>>8;
  uint8 least_significant_byte = address;
  spiPtr->send(most_significant_byte);      
  spiPtr->send(least_significant_byte);
}

boolean EEPROM25xxx::eepromWriteInProgress(void) {
  return eepromReadStatusRegisterBit(WIP_MASK);
}

boolean EEPROM25xxx::eepromWriteEnabled(void) {
  return eepromReadStatusRegisterBit(WEL_MASK);
}

boolean EEPROM25xxx::eepromReadStatusRegisterBit(uint8 mask) {
  uint8 data;
  eepromEnable();
  spiPtr->send(RDSR); 
  data = spiPtr->send(DUMMY_DATA);
  eepromDisable();
  if (data && mask) {
    return true;
  } else {
    return false;
  }
}

void EEPROM25xxx::eepromDisable(void) {
  digitalWrite(chipSelectPin, HIGH);    
}

void EEPROM25xxx::eepromEnable(void) {
  digitalWrite(chipSelectPin, LOW); 
}

