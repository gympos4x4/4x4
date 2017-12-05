// Author: Juraj Marcin

#include "Joystick.h"

Joystick::Joystick(uint16_t addressShift, uint8_t analogPinX, uint8_t analogPinY) {
	address = addressShift;
	xpin = analogPinX;
	ypin = analogPinY;
	/*cU = EEPROMExt.read16(address + JOY_ADS_U);
	cD = EEPROMExt.read16(address + JOY_ADS_D);
	cR = EEPROMExt.read16(address + JOY_ADS_R);
	cL = EEPROMExt.read16(address + JOY_ADS_L);
	cY = EEPROMExt.read16(address + JOY_ADS_Y);
	cX = EEPROMExt.read16(address + JOY_ADS_X);*/
	if (cU == 0) {
		cU = JOY_MIN;
	}
	if (cU == 0) {
		cD = JOY_MAX;
	}
	if (cR == JOY_MIN) {
		cR = JOY_MIN;
	}
	if (cL == 0) {
		cL = JOY_MAX;
	}
	if (cY == 0) {
		cY = abs(JOY_MAX - JOY_MIN) / 2;
	}
	if (cX == 0) {
		cX = abs(JOY_MAX - JOY_MIN) / 2;
	}
}

int8_t Joystick::readXint8() {
	int value = analogRead(xpin);
	if(abs(value - cX) < JOY_DEAD_ZONE) return JOY_OUT_C;
	if (value > cX) {
		return map(value, cX, cR, JOY_OUT_C, JOY_OUT_UR);
	} if (value < cX) {
		return map(value, cL, cX,JOY_OUT_DL, JOY_OUT_C);
	}
		return JOY_OUT_C;
}

int8_t Joystick::readYint8() {
	int value = analogRead(ypin);
	if(abs(value - cY) < JOY_DEAD_ZONE) return JOY_OUT_C;
	if (value > cY) {
		return map(value, cY, cU, JOY_OUT_C, JOY_OUT_UR);
	} if (value < cY) {
		return map(value, cD, cY, JOY_OUT_DL, JOY_OUT_C);
	}
		return JOY_OUT_C;
}

int16_t Joystick::readXraw() {
	return analogRead(xpin);
}

int16_t Joystick::readYraw() {
	return analogRead(ypin);
}

void Joystick::calibrateUp() {
	cU = readYraw();
	EEPROMExt.write16(address + JOY_ADS_U, cU);
}

void Joystick::calibrateDown() {
	cD = readYraw();
	EEPROMExt.write16(address + JOY_ADS_D, cD);
}

void Joystick::calibrateRight() {
	cR = readXraw();
	EEPROMExt.write16(address + JOY_ADS_R, cR);
}

void Joystick::calibrateLeft() {
	cL = readXraw();
	EEPROMExt.write16(address + JOY_ADS_L, cL);
}

void Joystick::calibrateCenter() {
	cY = readYraw();
	cX = readXraw();
	EEPROMExt.write16(address + JOY_ADS_Y, cY);
	EEPROMExt.write16(address + JOY_ADS_X, cX);
}