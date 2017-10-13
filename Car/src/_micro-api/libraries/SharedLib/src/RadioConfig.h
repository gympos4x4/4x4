#pragma once

#ifndef _RADIO_CONFIG_h
#define _RADIO_CONFIG_h

struct RadioConfig
{
	unsigned int pin_reset;
	unsigned int pin_cs;
	unsigned int interrupt;		//not pin, interrupt
	unsigned int pin_interrupt; //this time it's pin

	unsigned int pan;
	unsigned int my_address;
	unsigned int buddy_address;
};

const RadioConfig leo_radio_config {  8, 10, 0,  3, 0x1337, 0x6002 };
const RadioConfig mega_radio_config{ 13, 11, 0, 21, 0x1337, 0x6000 };
const RadioConfig uno_radio_config { 13, 11, 1, 255,0x1337, 0x6001 };

#endif
