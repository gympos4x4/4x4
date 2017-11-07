// Author: Juraj Marcin

#define SELECT_BTN_PIN 1
#define CALIB_BTN_PIN 0
#define LAX_PIN A7
#define LAY_PIN A6
#define RAX_PIN A1
#define RAY_PIN A0

#include <EEPROM.h>
#include <SPI.h>
#include <FT_Breakout_4DLCD_FT843.h>

#include "EEPROMExt.h"
#include "ControllerDisplay.h"
#include "Button.h"
#include "Joystick.h"

Button selectBtn(SELECT_BTN_PIN);
Button calibBtn(CALIB_BTN_PIN);
Joystick leftAnalog(0, LAX_PIN, LAY_PIN);
Joystick rightAnalog(12, RAX_PIN, RAY_PIN);

void setup() {
	ControllerDisplay.initDisplay();
}

void loop() {
	selectBtn.cycle();
	calibBtn.cycle();
	if (selectBtn.GetButtonDown()) {
		//TODO: toggle RF channel
	}
	
	if (calibBtn.GetButtonDown()) {
		calibrateLeftAnalog();
		calibrateRightAnalog();
	}
	
	int8_t throttle = rightAnalog.readYint8();
	int8_t steer = leftAnalog.readXint8();
	
	// RF_PINS: CS: PB2, RES: PB1, INT: PD2
	
	//TODO: update controller data
	//TODO: send controller data
	//TODO: receive car data
	
	//MISC
	int8_t battery;
	//TiltAlarm
	int16_t tilt_degrees;
	//Lights
	bool is_below_treshold;
	//ParkingSensors
	int16_t sersor0;
	int16_t sersor1;
	int16_t sersor2;
	int16_t sersor3;
	int16_t sersor4;
	int16_t sersor5;
	int16_t sersor6;
	int16_t sersor7;

	ControllerDisplay.startDrawing();
	ControllerDisplay.drawMainGUI();
	ControllerDisplay.drawCarBattery(battery);
	ControllerDisplay.drawFrontParkingSensors(sersor0, sersor1, sersor2, sersor3);
	ControllerDisplay.drawLights(is_below_treshold);
	ControllerDisplay.drawRearParkingSensors(sersor4, sersor5, sersor6, sersor7);
	ControllerDisplay.drawRotation(tilt_degrees);
	ControllerDisplay.drawSpeedometer(throttle);
	ControllerDisplay.drawWheels(steer);
	ControllerDisplay.endDrawing();
	delay(80);
}

void calibrateLeftAnalog() {
	leftAnalog.calibrateCenter();
	bool calibrated = false;
	//calibrate up
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(0, 0);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			leftAnalog.calibrateUp();
			calibrated = true;
		}
		delay(80);
	}
	calibrated = false;
	//calibrate down
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(0, 1);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			leftAnalog.calibrateDown();
			calibrated = true;
		}
		delay(80);
	}
	calibrated = false;
	//calibrate right
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(0, 2);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			leftAnalog.calibrateRight();
			calibrated = true;
		}
		delay(80);
	}
	calibrated = false;
	//calibrate left
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(0, 3);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			leftAnalog.calibrateLeft();
			calibrated = true;
		}
		delay(80);
	}
	calibrated = false;
}

void calibrateRightAnalog() {
	rightAnalog.calibrateCenter();
	bool calibrated = false;
	//calibrate up
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(1, 0);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			rightAnalog.calibrateUp();
			calibrated = true;
		}
		delay(80);
	}
	calibrated = false;
	//calibrate down
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(1, 1);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			rightAnalog.calibrateDown();
			calibrated = true;
		}
		delay(80);
	}
	calibrated = false;
	//calibrate right
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(1, 2);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			rightAnalog.calibrateRight();
			calibrated = true;
		}
		delay(80);
	}
	calibrated = false;
	//calibrate left
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(1, 3);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			rightAnalog.calibrateLeft();
			calibrated = true;
		}
		delay(80);
	}
	calibrated = false;
}