/*

eeprom_25xxx.h
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

#ifndef eeprom_25xxx_h
#define eeprom_25xxx_h

#ifdef __cplusplus
extern "C" {
#endif

#define EEPROM_SPI1_9MHZ SPI_PRESCALE_4
#define EEPROM_SPI2_9MHZ SPI_PRESCALE_8

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

void eeprom_25xxx_begin(uint32 spi_num, uint8 chip_select_pin);
void eeprom_25xxx_write(uint32 spi_num, uint8 chip_select_pin, uint16 address, uint8 data);
uint8 eeprom_25xxx_read(uint32 spi_num, uint8 chip_select_pin, uint16 address);
void eeprom_25xxx_send_address(uint32 spi_num, uint16 address);
boolean eeprom_25xxx_write_in_progress(uint32 spi_num, uint8 chip_select_pin);
boolean eeprom_25xxx_write_enabled(uint32 spi_num, uint8 chip_select_pin);
boolean eeprom_25xxx_read_status_register_bit(uint32 spi_num, uint8 chip_select_pin, uint8 mask);
void eeprom_25xxx_disable(uint8 chip_select_pin);
void eeprom_25xxx_enable(uint8 chip_select_pin);

#ifdef __cplusplus
}
#endif

#endif

