#define CAR

#include <stdbool.h>
#include <stdint.h>
#include <SPI.h>

#include "mrf24j.h"
#include "comm.h"

//controls
#include "car_devices/SteeringControl.h"
#include "car_devices/MotorControl.h"
//sensors
#include "car_devices/ParkingSensors.h"
#include "car_devices/TiltAlarm.h"
#include "car_devices/Lights.h"

Mrf24j mrf(9, -1, 2);
CarData cardata;
CtrlData ctrldata;
const unsigned int SYNC_INTERVAL_MS = 200;

unsigned long sync_last_time = 0;
bool rxl = 0;

void setup()
{
	setup_mrf(0x6001, 0xcafe);

	MotorControl::init();
	Lights.init(50);

	ParkingSensors.init();
	SteeringControl.init();
	TiltAlarm.init();

	pinMode(10,OUTPUT);
	sei();
}

void loop()
{
	//check if a new message came and update CarData and CtrlData if necessary
	mrf.check_flags(&mrf_rx, &mrf_tx);

	//use them data
	MotorControl::set_throttle(ctrldata.throttle);
	SteeringControl.steer(ctrldata.steer);

	TiltAlarm.loop();
	ParkingSensors.loop();
	Lights.loop();

	unsigned long current_time = millis();
	if(current_time - sync_last_time >= SYNC_INTERVAL_MS)
	{
		update_cardata();

		mrf.start_tx(0x4202, sizeof(cardata));
		mrf.send_car_data(&cardata);
		mrf.finish_tx();

		sync_last_time = current_time;
	}
	digitalWrite(10,rxl);
}

void mrf_isr()
{
	mrf.interrupt_handler();
}
void mrf_rx()
{
	mrf.recv_ctrl_data(&ctrldata);
	rxl = !rxl;
}
void mrf_tx()
{

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

void update_cardata()
{
	cardata.battery_percentage = -1;
	Lights.update_cardata(cardata);
	SteeringControl.update_cardata(cardata);
	TiltAlarm.update_cardata(cardata);
	ParkingSensors.update_cardata(cardata);
}