/*
SPI_EEPROM_benchmark.pde
AT25xxx EEPROM Benchmark Sketch for Maple

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

Based on code from EEPROM1024 library for Arduino:  
http://www.arduino.cc/playground/Code/I2CEEPROM24C1024
 
*/

#include <EEPROM25xxx.h>


// Set to a higher number if you want to start the test at a higher address. 
#define MIN_ADDRESS 0

// Upper boundary of the address space in bytes - choose the right one for your chip
#define MAX_ADDRESS 8192    // 25LC640
//#define MAX_ADDRESS 16384   // 25LC128
//#define MAX_ADDRESS 32768   // 25LC512
//#define MAX_ADDRESS 65536   // 25LC1024


#define CHIP_SELECT_PIN 10

// Use SPI 1:
//   Pin D11 MOSI
//   Pin D12 MISO 
//   Pin D13 sck
//   Pin D10 ss

#define SPI1 1

EEPROM25xxx eeprom(CHIP_SELECT_PIN, SPI1);

byte eeprom_output_data;
byte eeprom_input_data=0;

unsigned long time;
unsigned long finishTime;
unsigned long errors = 0;
unsigned long address = 0;
byte loop_size;

void setup() {
  // Make sure we aren't reading old data
  randomSeed(analogRead(0));
  loop_size = random(1, 100);

  SerialUSB.begin();
  SerialUSB.println("EEPROM 25xxx Library Benchmark Sketch");
  SerialUSB.println();

  eeprom.begin();

}  


void loop() {
  writeByByteTest();
  readByByteTest();
  delay(5000);
}


void writeByByteTest()
{
  time = millis();
  errors = 0;
  SerialUSB.println("--------------------------------");
  SerialUSB.println("Write By Byte Test:");
  SerialUSB.println();
  SerialUSB.print("Writing data:");
  for (address = MIN_ADDRESS; address < MAX_ADDRESS; address++) {
    eeprom.write(address, (uint8)(address % loop_size));
    if (!(address % 256)) SerialUSB.print(".");
  }
  finishTime = millis() - time;
  SerialUSB.println("DONE");
  SerialUSB.print("Total Time (seconds): "); 
  SerialUSB.println((unsigned long)(finishTime / 1000));
  SerialUSB.print("Write operations per second: "); 
  SerialUSB.println((unsigned long)(MAX_ADDRESS / (finishTime / 1000))); 
  SerialUSB.println("--------------------------------");   
  SerialUSB.println();
}

void readByByteTest()
{
  time = millis();
  errors = 0;
  SerialUSB.println("--------------------------------");
  SerialUSB.println("Read By Byte Test:");
  SerialUSB.println();
  SerialUSB.print("Reading data:");
  for (address = MIN_ADDRESS; address < MAX_ADDRESS; address++) {
    uint8 data;
    data = eeprom.read(address);
    if (data != (uint8)(address % loop_size)) {
      SerialUSB.println();
      SerialUSB.print("Address: ");
      SerialUSB.print(address);
      SerialUSB.print(" Should be: ");
      SerialUSB.print((uint8)(address % loop_size), DEC);
      SerialUSB.print(" Read val: ");
      SerialUSB.println(data, DEC);
      errors++;
    }
    if (!(address % 256)) SerialUSB.print(".");
  }
  finishTime = millis() - time;
  SerialUSB.println("DONE");
  SerialUSB.println();
  SerialUSB.print("Total Test Time (secs): "); 
  SerialUSB.println((unsigned long)(finishTime / 1000));
  SerialUSB.print("Read operations per second: "); 
  SerialUSB.println((unsigned long)(MAX_ADDRESS / (finishTime / 1000))); 
  SerialUSB.print("Total errors: "); 
  SerialUSB.println(errors);   
  SerialUSB.println("--------------------------------");
  SerialUSB.println();
}

