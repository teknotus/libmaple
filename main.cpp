// Sample main.cpp file. Blinks an LED, sends a message out USART2
// and turns on PWM on pin 2

#include "wirish.h"

#define LED_PIN 3

#include "libraries/Wire/Wire.h" // Maple software I2C library
#define EEPROM_SDA 1
#define EEPROM_SCL 2
#define EEPROM_DEVICE_ADDRESS 0x50

void i2c_eeprom_write_uint8(uint16 deviceAddress, uint16 dataAddress, uint8 data)
{
   Wire.beginTransmission((uint8) deviceAddress); // B1010xxx
   Wire.send((uint8)(dataAddress >> 8)); // MSB
   Wire.send((uint8)(dataAddress & 0xFF)); // LSB
   Wire.send(data);
   Wire.endTransmission();
   delay(5);
}

uint8 i2c_eeprom_read_uint8(uint16 deviceAddress, uint16 dataAddress)
{
   int n = 0;
   uint8 data = 65; // A - change me back to 0
   Wire.beginTransmission((uint8) deviceAddress); // B1010xxx
   Wire.send((uint8)(dataAddress >> 8)); // MSB
   Wire.send((uint8)(dataAddress & 0xFF)); // LSB
   int returnCode = Wire.endTransmission();
   SerialUSB.print("returnCode: ");
   SerialUSB.println(returnCode);
   n = Wire.requestFrom((uint8) deviceAddress,1);
   SerialUSB.print(":");
   SerialUSB.println(n);
   if (Wire.available()) data = Wire.receive();
   return data;
}

/*
void i2c_eeprom_write_uint8(int deviceAddress, unsigned int eeaddress, uint8 data) {
	int rdata = data;
	Wire.beginTransmission(deviceAddress);
	Wire.send((int) (eeaddress >> 8)); // MSB
	Wire.send((int) (eeaddress & 0xFF)); // LSB
	Wire.send(rdata);
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 uint8s, because the Wire library has a buffer of 32 uint8s
void i2c_eeprom_write_page(int deviceAddress, unsigned int eeaddresspage,
		uint8* data, uint8 length) {
	Wire.beginTransmission(deviceAddress);
	Wire.send((int) (eeaddresspage >> 8)); // MSB
	Wire.send((int) (eeaddresspage & 0xFF)); // LSB
	uint8 c;
	for (c = 0; c < length; c++) {
		Wire.send(data[c]);
	}
}

uint8 i2c_eeprom_read_uint8(int deviceAddress, unsigned int eeaddress) {
	uint8 rdata = 0xFF;
	Wire.beginTransmission(deviceAddress);
	Wire.send((int) (eeaddress >> 8)); // MSB
	Wire.send((int) (eeaddress & 0xFF)); // LSB
	Wire.send(deviceAddress || 1);
	if (Wire.available()) {
		rdata = Wire.receive();
	}
	return rdata;
}

// maybe let's not read more than 30 or 32 uint8s at a time!
void i2c_eeprom_read_buffer(int deviceAddress, unsigned int eeaddress,
		uint8 *buffer, int length) {
	Wire.beginTransmission(deviceAddress);
	Wire.send((int) (eeaddress >> 8)); // MSB
	Wire.send((int) (eeaddress & 0xFF)); // LSB
	Wire.send(length);
	int c = 0;
	for (c = 0; c < length; c++) {
		if (Wire.available()) {
			buffer[c] = Wire.receive();
		}
	}
}


*/

void setup() {
	/* Set up the LED to blink  */
	pinMode(LED_PIN, OUTPUT);

	/* Send a message out the usb virtual serial port  */
	SerialUSB.println("Hello!");

	char somedata[] = "this is data from the eeprom"; // data to write
	Wire.begin(EEPROM_SDA, EEPROM_SCL); // initialize the connection
	//i2c_eeprom_write_page(EEPROM_ADDRESS, 0, (uint8 *) somedata,
	//		sizeof(somedata)); // write to EEPROM
	i2c_eeprom_write_uint8(EEPROM_DEVICE_ADDRESS, 0, (uint8) 'y');
	i2c_eeprom_write_uint8(EEPROM_DEVICE_ADDRESS, 1, (uint8) 'n');
	i2c_eeprom_write_uint8(EEPROM_DEVICE_ADDRESS, 2, (uint8) 0);

	delay(10); //add a small delay

	SerialUSB.println("Memory written");

}

int toggle = 0;

void loop() {
	toggle ^= 1;
	digitalWrite(LED_PIN, toggle);
	delay(100);

	//int addr = 0; //first address
	i2c_eeprom_write_uint8(EEPROM_DEVICE_ADDRESS, 0, (uint8) 'y');
	uint8 b = i2c_eeprom_read_uint8(EEPROM_DEVICE_ADDRESS, 0); // access the first address from the memory
	SerialUSB.print((char) b, 16); //print content to serial port

	//while (b != 0) {
		//SerialUSB.print((char) b); //print content to serial port
		//addr++; //increase address
    //  b = i2c_eeprom_read_uint8(EEPROM_DEVICE_ADDRESS, addr); //access an address from the memory
	//}
	SerialUSB.println(" ");
	delay(2000);

}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated object that need libmaple may fail.
__attribute__(( constructor )) void premain() {
	init();
}

int main(void) {
	setup();

	while (1) {
		loop();
	}
	return 0;
}

