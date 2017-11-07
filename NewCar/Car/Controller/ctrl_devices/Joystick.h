// Author: Juraj Marcin

#ifndef _JOYSTICK_h
#define _JOYSTICK_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define JOY_MIN 0
#define JOY_MAX 1024
#define JOY_OUT_DL -128
#define JOY_OUT_UR 128
#define JOY_OUT_C 0
#define JOY_ADS_U 0
#define JOY_ADS_D 2
#define JOY_ADS_R 4
#define JOY_ADS_L 6
#define JOY_ADS_Y 8
#define JOY_ADS_X 10

#include "EEPROMExt.h"

class Joystick {

	private:
	
	uint16_t address;
	uint8_t xpin, ypin;
	uint16_t cU, cD, cR, cL, cY, cX;

	public:

	Joystick(uint16_t addressShift, uint8_t analogPinX, uint8_t analogPinY);
	~Joystick() {}

	int8_t readXint8();

	int8_t readYint8();

	int16_t readXraw();

	int16_t readYraw();
	
	void calibrateUp();
	
	void calibrateDown();
	
	void calibrateRight();
	
	void calibrateLeft();
	
	void calibrateCenter();
};

#endif

