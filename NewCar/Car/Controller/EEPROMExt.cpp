// Author: Juraj Marcin

#include "EEPROMExt.h"

_EEPROMExt EEPROMExt;

void _EEPROMExt::write16(uint16_t address, uint16_t value) {
	uint8_t two = (value & 0xFF);
	uint8_t one = ((value >> 8) & 0xFF);

	EEPROM.write(address, two);
	EEPROM.write(address + 1, one);
}

uint16_t _EEPROMExt::read16(uint16_t address) {
	uint16_t two = EEPROM.read(address);
	uint16_t one = EEPROM.read(address + 1);

	return ((two << 0) & 0xFF) + ((one << 8) & 0xFFFF);
}

void _EEPROMExt::write32(uint32_t address, uint32_t value) {
	uint8_t two = (value & 0xFF);
	uint8_t one = ((value >> 8) & 0xFF);

	EEPROM.write(address, two);
	EEPROM.write(address + 1, one);
}

uint32_t _EEPROMExt::read32(uint32_t address) {
	uint32_t four = EEPROM.read(address);
	uint32_t three = EEPROM.read(address + 1);
	uint32_t two = EEPROM.read(address + 2);
	uint32_t one = EEPROM.read(address + 3);

	return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}