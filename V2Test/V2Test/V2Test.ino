/*
* V2Test.ino
*
* Created: 11/4/2017 5:06:28 PM
* Author: DELTA-PC
*/
#include <SPI>
#include <Servo.h>
#include "comm.h"
#include "mrf24j.h"
#include "MotorControl.h"

Mrf24j mrf(/*pin reset*/ 4, /*pin CS*/ 9, /*pin itnerrupt*/ 3);

CarData cardata;
CtrlData ctrldata;
void mrf_isr()
{
	mrf.interrupt_handler();
}
Servo servo, servo2;
void setup()
{
	servo.attach(5);
	servo2.attach(6);
	mrf.reset();
	mrf.init();
	mrf.set_pan(0xcafe);
	mrf.address16_write(0x6001);
	mrf.set_promiscuous(true);
	mrf.set_palna(true);
	attachInterrupt(1, mrf_isr, CHANGE); // interrupt 1 equivalent to pin 3(INT1) on ATmega8/168/328
	/* add setup code here, setup code runs once when the processor starts */
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
	sei();
	cardata.tilt.tilt_degrees = 50;
	//	DDRB |= 1 << PINB1;
	MotorControl.init();
	pinMode(1, OUTPUT);
	pinMode(5,OUTPUT);
}
char rxl = 0;
char txl = 0;
unsigned long _time = 0;
unsigned long lastTime = 0;
void loop()
{
	_time = millis();
	mrf.check_flags(&mrf_rx, &mrf_tx);
	
	if(_time - lastTime > 50){
		mrf.read_rxdata();
		mrf.recv_ctrl_data(&ctrldata);
		lastTime = _time;
		cardata.throttleFb = 86;
		mrf.start_tx(0x6000, sizeof(cardata));
		mrf.send_car_data(&cardata);
		mrf.finish_tx();
		if (rxl)
		{
			rxl = 0;
			digitalWrite(1,LOW);
		}
		else// if(!mrf.get_txinfo()->tx_ok)
		{
			rxl = 1;
			digitalWrite(1,HIGH);
		}
	}
	
	MotorControl.loop(ctrldata,_time);
}

void mrf_rx()
{
}
void mrf_tx()
{

}
