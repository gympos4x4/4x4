#pragma once

#ifndef _RADIO_CONFIG_h
#define _RADIO_CONFIG_h

struct RadioConfig
{
	unsigned int pin_reset;
	unsigned int pin_cs;
	unsigned int interrupt;		//not pin, interrupt

	unsigned int pan;
	unsigned int my_address;
	unsigned int buddy_address;
};

const RadioConfig mega_radio_config{ 13, 11, 0, 0x1337, 0x6000 };
const RadioConfig uno_radio_config{ 13, 11, 1, 0x1337, 0x6001 };
const RadioConfig leo_radio_config{ 8, 10, 0, 0x1337, 0x6002 };

#endif
