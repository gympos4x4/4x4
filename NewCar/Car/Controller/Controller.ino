// Author: Juraj Marcin
#define CTRL

#define SELECT_BTN_PIN 6
#define CALIB_BTN_PIN 5
#define AX_PIN A2
#define AY_PIN A4
#define MRF_RES_PIN 15 //PC1
#define MRF_CS_PIN 16 //PC0
#define MRF_INT_PIN 3 //PD3(INT1)
#define MRF_INT_APIN 1 //INT1(PD3)
 

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
Joystick analog(0, AX_PIN, AY_PIN);

const unsigned int FPS = 48;
unsigned long redraw_last_time = 0;
unsigned long current_time = 0;

Mrf24j mrf(/*pin reset*/ MRF_RES_PIN, /*pin CS*/ MRF_CS_PIN, /*pin itnerrupt*/ MRF_INT_PIN);

void setup() {
	//ControllerDisplay.initDisplay();

	setup_mrf(0x6000, 0xcafe);
	sei();
}

void loop() {
	current_time = millis();
	//check if a new message came and update CarData if necessary
	//mrf.check_flags(&mrf_rx, &mrf_tx);

	/*selectBtn.cycle();
	calibBtn.cycle();
	if (selectBtn.GetButtonDown()) {
		//TODO: toggle RF channel
	}
	
	if (calibBtn.GetButtonDown()) {
		calibrateAnalog();
	}*/
	
	ctrldata.throttle = analog.readYint8();
	ctrldata.steer = analog.readXint8();

	//TODO: send controller data
	unsigned long current_time = millis();
	if(current_time - sync_last_time >= SYNC_INTERVAL_MS)
	{
		mrf.read_rxdata();
		mrf.recv_car_data(&cardata);

		mrf.start_tx(0x6001, sizeof(ctrldata));
		mrf.send_ctrl_data(&ctrldata);
		mrf.finish_tx();

		sync_last_time = current_time;
	}

	//don't delay so we can sync data as frequently as possible, but don't waste time redrawing every loop

	/*if( current_time - redraw_last_time >= (1000/FPS) )
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
	}*/
}

void calibrateAnalog() {
	analog.calibrateCenter();
	bool calibrated = false;
	//calibrate up (RA)
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(1, 0);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			analog.calibrateUp();
			calibrated = true;
		}
	}
	calibrated = false;
	//calibrate down (RA)
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(1, 1);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			analog.calibrateDown();
			calibrated = true;
		}
	}
	calibrated = false;
	//calibrate right (LA)
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(0, 2);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			analog.calibrateRight();
			calibrated = true;
		}
		delay(80);
	}
	calibrated = false;
	//calibrate left (LA)
	ControllerDisplay.startDrawing();
	ControllerDisplay.drawCalibrationNotice(0, 3);
	ControllerDisplay.endDrawing();
	while (!calibrated)	{
		if (calibBtn.GetButtonDown()) {
			analog.calibrateLeft();
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
	mrf.set_promiscuous(true);
	//attachInterrupt(MRF_INT_APIN, mrf_isr, CHANGE); // interrupt 1 equivalent to pin 3(INT1) on ATmega8/168/328
}

void mrf_isr()
{
	//mrf.interrupt_handler();
}
void mrf_rx()
{
	//mrf.recv_car_data(&cardata);
	//rxl = !rxl;		looks like a debug statement
}
void mrf_tx()
{

}