// Author: Juraj Marcin
#define CTRL

#define SELECT_BTN_PIN 1
#define CALIB_BTN_PIN 0
#define LAX_PIN A7
#define LAY_PIN A6
#define RAX_PIN A1
#define RAY_PIN A0

#include <EEPROM.h>
#include <SPI.h>
#include "FTDI/FT_Breakout_4DLCD_FT843.h"

#include "EEPROMExt.h"
#include "ctrl_devices/ControllerDisplay.h"
#include "ctrl_devices/Button.h"
#include "ctrl_devices/Joystick.h"

#include "mrf24j.h"
#include "comm.h"
CarData cardata;
CtrlData ctrldata;
const unsigned int SYNC_INTERVAL_MS = 20;
unsigned long sync_last_time = 0;

Button selectBtn(SELECT_BTN_PIN);
Button calibBtn(CALIB_BTN_PIN);
Joystick leftAnalog(0, LAX_PIN, LAY_PIN);
Joystick rightAnalog(12, RAX_PIN, RAY_PIN);

const unsigned int FPS = 48;
unsigned long redraw_last_time = 0;

// RF_PINS: CS: PB2, RES: PB1, INT: PD2
Mrf24j mrf(/*pin reset*/ 15, /*pin CS*/ 16, /*pin itnerrupt*/ 2);

void setup() {
	ControllerDisplay.initDisplay();

	setup_mrf(0x6000, 0xcafe);
	sei();
}

void loop() {
	//check if a new message came and update CarData if necessary
	mrf.check_flags(&mrf_rx, &mrf_tx);

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
	
	//update controller data
	ctrldata.steer = steer;
	ctrldata.throttle = throttle;

	//TODO: send controller data
	unsigned long current_time = millis();
	if(current_time - sync_last_time >= SYNC_INTERVAL_MS)
	{
		mrf.start_tx(0x6001, sizeof(ctrldata));
		mrf.send_ctrl_data(&ctrldata);
		mrf.finish_tx();

		sync_last_time = current_time;
	}

	//don't delay so we can sync data as frequently as possible, but don't waste time redrawing every loop
	current_time = millis();
	if( current_time - redraw_last_time >= (1000/FPS) )
	{
		ControllerDisplay.startDrawing();
		ControllerDisplay.drawMainGUI();
		ControllerDisplay.drawCarBattery(cardata.battery_percentage);
		ControllerDisplay.drawFrontParkingSensors(cardata.parking.sensor_data[0], cardata.parking.sensor_data[1], cardata.parking.sensor_data[2], cardata.parking.sensor_data[3]);
		ControllerDisplay.drawLights(cardata.lights.is_below_threshold);
		ControllerDisplay.drawRearParkingSensors(cardata.parking.sensor_data[4], cardata.parking.sensor_data[5], cardata.parking.sensor_data[6], cardata.parking.sensor_data[7]);
		ControllerDisplay.drawRotation(cardata.tilt.tilt_degrees);
		ControllerDisplay.drawSpeedometer(throttle);
		ControllerDisplay.drawWheels(steer);
		ControllerDisplay.endDrawing();

		redraw_last_time = current_time;
	}
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

void setup_mrf(word address, word pan)
{
	mrf.reset();
	mrf.init();
	mrf.set_pan(pan);
	mrf.address16_write(address);
	mrf.set_palna(true);
	attachInterrupt(0, mrf_isr, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
}

void mrf_isr()
{
	mrf.interrupt_handler();
}
void mrf_rx()
{
	mrf.recv_car_data(&cardata);
	//rxl = !rxl;		looks like a debug statement
}
void mrf_tx()
{

}