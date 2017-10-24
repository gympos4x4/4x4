// Author: Juraj Marcin

#include "eADC.h"

void eADC::init() {
	pinMode(ADC_CSPIN, OUTPUT);
	digitalWrite(ADC_CSPIN, HIGH);
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV8);
}

uint16_t eADC::analogRead(int channel) {
	uint8_t msb;
	uint8_t lsb;

	// shift bits to match datasheet for MCP3208
	byte commandMSB = B00000110;
	uint16_t commandBytes = (uint16_t)(commandMSB << 8 | channel << 6);

	// select ADC
	digitalWrite(9, LOW);
	// send start bit and bit to specify single or differential mode (single mode chosen here)
	SPI.transfer((commandBytes >> 8) & 0xff);

	msb = SPI.transfer((byte)commandBytes & 0xff) & B00001111;
	lsb = SPI.transfer(0x00);

	digitalWrite(9, HIGH);


	// cast before shifting the byte
	return ((uint16_t)msb) << 8 | lsb;
}
