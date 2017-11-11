
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

void setup() {
	Serial.begin(9600);

	mrf.reset();
	mrf.init();
	//mrf.set_promiscuous(true);
	mrf.set_pan(0xcafe);
	mrf.address16_write(0x6000);
	mrf.set_palna(true);
	mrf.set_promiscuous(true);
	attachInterrupt(0, interrupt_routine, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
	last_time = millis();
	servo.attach(22);
	interrupts();
}
void interrupt_routine() {
	mrf.interrupt_handler(); // mrf24 object interrupt routine
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
		mrf.start_tx(0x6001,sizeof(ctrldata));
		mrf.send_ctrl_data(&ctrldata);
		mrf.finish_tx();
		lastTxTime = currentTime;
		mrf.read_rxdata();
		mrf.recv_car_data(&cardata);
		Serial.println((int)cardata.throttleFb);
	}
	if(currentTime - errorTime >= 1000){

		Serial.println("Error, stopped receiving");
		Serial.print("BBREG1: ");
		Serial.println((unsigned int)mrf.read_short(MRF_BBREG1));
		Serial.print("PAN: ");
		Serial.println((unsigned int)mrf.get_pan());
		Serial.print("ADDRESS: ");
		Serial.println((unsigned int)mrf.address16_read());
		/*mrf.address16_write(0x6000);
		mrf.set_pan(0xcafe);*/
		
		errorTime = currentTime;
	}
	/*if(currentTime - lastProgTime >= 5000)
	{
	lastProgTime = currentTime;
	switch(prog)
	{
	case 0:
	ctrldata.throttle = 0;
	ctrldata.steering = 30;
	break;
	case 1:
	ctrldata.throttle = 126;
	ctrldata.steering = -30;
	break;
	case 2:
	ctrldata.steering = 0;
	ctrldata.throttle = 127;
	break;
	case 3:
	ctrldata.throttle = 0;
	ctrldata.steering = 20;
	break;
	case 6:
	ctrldata.throttle = -64;
	break;
	}
	Serial.println(ctrldata.steering);
	Serial.print("Program ");
	Serial.println((int)prog);
	if(prog < 1) prog++;
	else prog = 0;
	}*/
	int ain = analogRead(A0);
	//uint8_t debug = map(ain,0,1012,5,35);
	int8_t debug = map(ain,0,800,-126,126);
	//ctrldata.steering = debug;
	ctrldata.throttle = debug;
	//Serial.println((int)debug);
	//servo.write(debug);
}

void handle_rx() {
	#ifdef LOG
	Serial.print("Received data: ");
	mrf.recv_car_data(&cardata);
	/*Serial.print("Tilt: ");
	Serial.println(cardata.tilt.tilt_degrees);
	Serial.print("Battery %: ");
	Serial.println(cardata.battery_percentage);*/
	Serial.print("Throttle fb: ");
	Serial.println(cardata.throttleFb);
	Serial.print("Steer fb: ");
	Serial.println(cardata.steerFb);
	Serial.print("LQI: ");
	Serial.println(mrf.get_rxinfo()->lqi);
	Serial.print("RSSI: ");
	Serial.println(mrf.get_rxinfo()->rssi);
	#endif
	//errorTime = currentTime;
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
