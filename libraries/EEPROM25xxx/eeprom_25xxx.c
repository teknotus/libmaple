/*

eeprom_25xxx.c
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
 * @brief Microchip 25xxx SPI EEPROM interface
 * 
 * This driver works with the following EEPROMs: 25xx640, 25xx128, 25xx512, 25xx1024 
 *
 * It only implements single-byte read and write.
 *
 * Chip enable and disable functions are provided as a convenience.
 *
 * Note: this library assumes that the program using the EEPROM has
 * exclusive access to the SPI bus. If that is not the case, the
 * caller has to use the begin() method to reinitialize the SPI
 * hardware with its speed, endianness, and mode
 *
 */

/*

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

*/


#include "libmaple.h"
#include "spi.h"
#include "wirish.h"
#include "eeprom_25xxx.h"


/**
 * @brief Initialize the EEPROM and the SPI port
 * @param spi_num number of the spi port the EEPROM is connected to, 1 or 2
 * @param chip_select_pin number of the pin the EEPROM's chip select pin is connected to
 */
void eeprom_25xxx_begin(uint32 spi_num, uint8 chip_select_pin) {
    pinMode(chip_select_pin, OUTPUT);
    eeprom_25xxx_disable(chip_select_pin);

    uint32 prescale;
    if (spi_num == 1) {
        prescale = EEPROM_SPI1_9MHZ;
    } else {
        prescale = EEPROM_SPI2_9MHZ;
    }

    spi_init(spi_num, prescale, SPI_MSBFIRST, SPI_MODE_0);
}

/**
 * @brief Write a single byte to the EEPROM
 * @param spi_num number of the spi port the EEPROM is connected to, 1 or 2
 * @param chip_select_pin number of the pin the EEPROM's chip select pin is connected to
 * @param address EEPROM memory address to access
 * @param data byte of data to write 
 */
void eeprom_25xxx_write(uint32 spi_num, uint8 chip_select_pin, uint16 address, uint8 data) {
    eeprom_25xxx_enable(chip_select_pin);
    spi_tx_byte(spi_num, WREN); 
    eeprom_25xxx_disable(chip_select_pin); // Needed for EEPROM to accept write enable (WREN) opcode
    eeprom_25xxx_enable(chip_select_pin);
    spi_tx_byte(spi_num, WRITE); 
    eeprom_25xxx_send_address(spi_num, address);
    spi_tx_byte(spi_num, data); 
    eeprom_25xxx_disable(chip_select_pin); // Needed for EEPROM to initiate write cycle
    eeprom_25xxx_enable(chip_select_pin);
    while (eeprom_25xxx_write_in_progress(spi_num, chip_select_pin)) {
	// do nothing - busy wait, otherwise we get write errors
    }
    eeprom_25xxx_disable(chip_select_pin);
}

/**
 * @brief Read a single byte from the EEPROM
 * @param spi_num number of the spi port the EEPROM is connected to, 1 or 2
 * @param chip_select_pin number of the pin the EEPROM's chip select pin is connected to
 * @param address EEPROM memory address to access
 */
uint8 eeprom_25xxx_read(uint32 spi_num, uint8 chip_select_pin, uint16 address) {
  uint8 data;
  eeprom_25xxx_enable(chip_select_pin);
  spi_tx_byte(spi_num, READ); 
  eeprom_25xxx_send_address(spi_num, address);
  data = spi_tx_byte(spi_num, DUMMY_DATA);
  eeprom_25xxx_disable(chip_select_pin);
  return data;
}

/**
 * @brief Send an address to the EEPROM via the SPI port
 * @param spi_num number of the spi port the EEPROM is connected to, 1 or 2
 * @param address EEPROM memory address to send
 */
void eeprom_25xxx_send_address(uint32 spi_num, uint16 address) {
    uint8 most_significant_byte = address>>8;
    uint8 least_significant_byte = address;
    spi_tx_byte(spi_num, most_significant_byte);      
    spi_tx_byte(spi_num, least_significant_byte);
}

/**
 * @brief Query the EEPROM to see if a write is in progress
 * @param spi_num number of the spi port the EEPROM is connected to, 1 or 2
 * @param chip_select_pin number of the pin the EEPROM's chip select pin is connected to
 */
boolean eeprom_25xxx_write_in_progress(uint32 spi_num, uint8 chip_select_pin) {
    return eeprom_25xxx_read_status_register_bit(spi_num, chip_select_pin, WIP_MASK);
}

/**
 * @brief Query the EEPROM to see if it is ready to be written to
 * @param spi_num number of the spi port the EEPROM is connected to, 1 or 2
 * @param chip_select_pin number of the pin the EEPROM's chip select pin is connected to
 */
boolean eeprom_25xxx_write_enabled(uint32 spi_num, uint8 chip_select_pin) {
    return eeprom_25xxx_read_status_register_bit(spi_num, chip_select_pin, WEL_MASK);
}

/**
 * @brief Read a single status register bit
 * @param spi_num number of the spi port the EEPROM is connected to, 1 or 2
 * @param chip_select_pin number of the pin the EEPROM's chip select pin is connected to
 * @param mask bit mask that is ANDed to the status register to test a particular bit
 */
boolean eeprom_25xxx_read_status_register_bit(uint32 spi_num, uint8 chip_select_pin, uint8 mask) {
    uint8 data;
    eeprom_25xxx_enable(chip_select_pin);
    spi_tx_byte(spi_num, RDSR); 
    data = spi_tx_byte(spi_num, DUMMY_DATA);
    eeprom_25xxx_disable(chip_select_pin);
    if (data && mask) {
	return true;
    } else {
	return false;
    }
}

/**
 * @brief Disable the EEPROM using the Chip Select line
 * @param chip_select_pin number of the pin the EEPROM's chip select pin is connected to
 */
void eeprom_25xxx_disable(uint8 chip_select_pin) {
  digitalWrite(chip_select_pin, HIGH);    
}

/**
 * @brief Enable the EEPROM using the Chip Select line
 * @param chip_select_pin number of the pin the EEPROM's chip select pin is connected to
 */
void eeprom_25xxx_enable(uint8 chip_select_pin) {
  digitalWrite(chip_select_pin, LOW); 
}

