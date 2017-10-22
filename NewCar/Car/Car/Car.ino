#define CAR

#include <stdbool.h>
#include <stdint.h>
#include <SPI.h>
#include "mrf24j.h"
#include "comm.h"
#include "MotorControl.h"

Mrf24j mrf(9,-1,2);
CarData cardata;
CtrlData ctrldata;

unsigned long currentTime = 0;
unsigned long lastTime = 0;
void setup()
{
	mrf.reset();
	mrf.init();
	mrf.set_pan(0xcafe);
	mrf.address16_write(0x6001);
	mrf.set_palna(true);
	attachInterrupt(0, mrf_isr, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
	cardata.tilt = 10;
	MotorControl::init();
	pinMode(10,OUTPUT);
	sei();
}
bool rxl = 0;
void loop()
{
	currentTime = millis();
	mrf.check_flags(&mrf_rx, &mrf_tx);
	if(currentTime - lastTime >= 200)
	{
		cardata.tilt = ctrldata.throttle;
		mrf.start_tx(0x4202, cardata.len);
		mrf.send_car_data(&cardata);
		mrf.finish_tx();
		lastTime = currentTime;
	}
	digitalWrite(10,rxl);
	MotorControl::set_throttle(ctrldata.throttle);
}

void mrf_isr()
{
	mrf.interrupt_handler();
}

void mrf_rx()
{
	mrf.recv_ctrl_data(&ctrldata);
	if(rxl)rxl = 0;
	else rxl = 1;
}
void mrf_tx()
{

}