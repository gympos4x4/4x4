// Author: Juraj Marcin
// ControllerDisplay.h


#ifndef _CONTROLLERDISPLAY_h
#define _CONTROLLERDISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define CD_CS_PIN 8
#define CD_PD_PIN 7

#define WHL_COLOR 0x0F75BC
#define WHL_MIN -100
#define WHL_MAX 100

#define SPD_COLOR 0x0F75BC
#define SPD_BG_COLOR 0x222222
#define SPD_MAJOR 10
#define SPD_MINOR 10
#define SPD_RANGE 100
//#define SPD_DRAW_TEXT

#define BAT_LOW_COL 0xFCB040
#define BAT_WAR_COL 0xED1C24

#define ROT_WAR_COL 0xED1C24
#define ROT_LOW_COL 0xFCB040
#define ROT_COLOR 0x0F75BC

#define LII_DAY_COLOR 0x00C411
#define LII_HEAD_COLOR 0x2C7DFF

#define PRS_FRONT_MIN 0
#define PRS_FRONT_NEAR 37
#define PRS_FRONT_MAX 45
#define PRS_REAR_MIN 272
#define PRS_REAR_NEAR 235
#define PRS_REAR_MAX 227
#define PRS_COLOR 0xFCB040
#define PRS_NEAR_COLOR 0xED1C24

#include <SPI.h>
#include <FT_Breakout_4DLCD_FT843.h>

class _ControllerDisplay {
	
	private:

	bool displayInitialized;
	FT800IMPL_SPI* FTImpl;

	uint16_t bootupConfigure();

	public:

	void initDisplay();

	void startDrawing();

	void drawMainGUI();

	void drawWheels(int8_t steer);

	void drawSpeedometer(int8_t speed);

	void drawCarBattery(int8_t percentage);

	void drawRotation(int8_t z);

	void drawLights(bool headlights);

	void drawFrontParkingSensors(int16_t sensor0, int16_t sensor1, int16_t sensor2, int16_t sensor3);

	void drawRearParkingSensors(int16_t sensor4, int16_t sensor5, int16_t sensor6, int16_t sensor7);
	
	void drawCalibrationNotice(int8_t joystick, int8_t direction);
	
	void drawSignalStrenght(uint8_t signalStrenght);

	void endDrawing();
};

extern _ControllerDisplay ControllerDisplay;

#endif