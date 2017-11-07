// Author: Juraj Marcin

#ifndef _EEPROMEXT_h
#define _EEPROMEXT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <EEPROM/EEPROM.h>

class _EEPROMExt {
	
	public:
	
	void write16(uint16_t address, uint16_t value);
	
	uint16_t read16(uint16_t address);
	
	void write32(uint32_t address, uint32_t value);

	uint32_t read32(uint32_t address);
};

extern _EEPROMExt EEPROMExt;

#endif

