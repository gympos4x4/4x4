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
void loop()
{
	mrf.check_flags(&mrf_rx, &mrf_tx);
	/*mrf.start_tx(0x6000, sizeof(cardata));
	mrf.send_car_data(&cardata);
	mrf.finish_tx();*/
	
	/*digitalWrite(A3, LOW);
	digitalWrite(A2, LOW);
	delay(3000);
	for (int i = 0; i < 600; i++)
	{
	digitalWrite(A3, LOW);
	digitalWrite(A2, LOW);
	delay(1);
	digitalWrite(A2,LOW);
	digitalWrite(A3,HIGH);
	delay(4);
	}
	digitalWrite(A2, LOW);
	digitalWrite(A3, HIGH);
	delay(3000);*/
	
	/*servo.write(50);
	servo2.write(-50);
	delay(1000);
	servo.write(-50);
	servo2.write(50);
	delay(1000);*/
	
	ctrldata.throttle = 126;
	MotorControl.loop(ctrldata,millis());
	
	//delay(100);
	/*	mrf.recv_ctrl_data(&ctrldata);
		if (txl)
		{
			txl = 0;
			digitalWrite(5,LOW);
		}
		else if(mrf.address16_read() == 0x6001 && mrf.get_pan())
		{
			txl = 1;
			digitalWrite(5,HIGH);
		}*/
}

void mrf_rx()
{

}
void mrf_tx()
{
	if(mrf.get_txinfo()->tx_ok)
	{
				if (rxl)
				{
					rxl = 0;
					digitalWrite(1,LOW);
				}
				else
				{
					rxl = 1;
					digitalWrite(1,HIGH);
				}
	}
	
}
