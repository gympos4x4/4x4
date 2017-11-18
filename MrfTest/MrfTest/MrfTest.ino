
#include "mrf24j.h"
//#include "mrf24j_c.h"
#include <SPI.h>
#include <Servo.h>
#include "comm.h"


Mrf24j mrf(34,24,2);
long last_time;
long tx_interval = 1000;
CarData cardata;
CtrlData ctrldata;
Servo servo;
#define LOG

void setup_mrf(word address, word pan)
{
	mrf.reset();
	mrf.init();
	mrf.set_pan(pan);
	mrf.address16_write(address);
	mrf.set_palna(true);
	mrf.set_promiscuous(true);
	attachInterrupt(4, interrupt_routine, CHANGE); // interrupt 4 equivalent to pin 2 on Arduino Mega
}

void setup() {
	Serial.begin(9600);

	setup_mrf(0x6001, 0xCAFE);
	
	last_time = millis();
	interrupts();
}

void interrupt_routine() {
	//mrf.interrupt_handler(); // mrf24 object interrupt routine
}

unsigned long currentTime = 0;
unsigned long lastTxTime = 0;
unsigned long errorTime = 0;
unsigned long lastProgTime = 0;
byte prog;
void loop() {
	mrf.check_flags(&handle_rx, &handle_tx);
	
	currentTime = millis();
	if(currentTime - lastTxTime >= 50){

	byte response = mrf.read_rxdata();
			Serial.print(("First byte: "));
			Serial.println((int)response);
	if(response == 42)
	{
		mrf.recv_ctrl_data(&ctrldata);

		Serial.print("throttle:");
		Serial.println((int)ctrldata.throttle);

		errorTime = currentTime;

		}

		//mrf.start_tx(0x6001,sizeof(ctrldata));
		//mrf.send_ctrl_data(&ctrldata);
		//mrf.finish_tx();
		//mrf.read_rxdata();
		//mrf.recv_car_data(&cardata);
		//Serial.println((int)cardata.throttleFb);

		lastTxTime = currentTime;
	}

	if(currentTime - errorTime >= 1000){

		Serial.println("Error, stopped receiving");
		Serial.print("BBREG1: ");
		Serial.println((unsigned int)mrf.read_short(MRF_BBREG1));
		Serial.print("PAN: ");
		Serial.println((unsigned int)mrf.get_pan());
		Serial.print("ADDRESS: ");
		Serial.println((unsigned int)mrf.address16_read());
		
		errorTime = currentTime;
	}
}

void handle_rx() {
}

void handle_tx() {
	if(!mrf.get_txinfo()->tx_ok)
	{
		Serial.println("TX failed!");
		if (mrf.get_txinfo()->channel_busy)
		{
			Serial.println("channel busy");
		}
	}
	else
	Serial.println("tx ok");
}
