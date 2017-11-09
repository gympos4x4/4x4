
#include "mrf24j.h"
//#include "mrf24j_c.h"
#include <SPI.h>
#include "comm.h"


Mrf24j mrf(34,24,2);
long last_time;
long tx_interval = 1000;
CarData cardata;
CtrlData ctrldata;

//#define LOG

void setup() {
	Serial.begin(9600);

	mrf.reset();
	mrf.init();
	//mrf.set_promiscuous(true);
	mrf.set_pan(0xcafe);
	mrf.address16_write(0x6000);
	mrf.set_palna(true);
	attachInterrupt(0, interrupt_routine, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
	last_time = millis();
	interrupts();
}
void interrupt_routine() {
	mrf.interrupt_handler(); // mrf24 object interrupt routine
}
unsigned long currentTime = 0;
unsigned long lastTxTime = 0;
unsigned long lastProgTime = 0;
byte prog;
void loop() {
	mrf.check_flags(&handle_rx, &handle_tx);
	
	currentTime = millis();
	if(currentTime - lastTxTime >= 100){
		mrf.start_tx(0x6001,sizeof(ctrldata));
		mrf.send_ctrl_data(&ctrldata);
		mrf.finish_tx();
		lastTxTime = currentTime;
	}
	if(currentTime - lastProgTime >= 5000)
	{
		lastProgTime = currentTime;
		switch(prog)
		{
			case 0:
			ctrldata.throttle = 0;
			ctrldata.steering.servo_angle = 30;
			break;
			case 1:
			ctrldata.steering.servo_angle = -30;
			break;
			case 2:
			ctrldata.steering.servo_angle = 0;
			ctrldata.throttle = 127;
			break;
			case 3:
			ctrldata.throttle = 0;
			ctrldata.steering.servo_angle = 20;
			break;
			case 6:
			ctrldata.throttle = -64;
			break;
		}
		Serial.print("Program ");
		Serial.println((int)prog);
		if(prog < 6) prog++;
		else prog = 0;
	}
}

void handle_rx() {
	#ifdef LOG
	Serial.print("Received data: ");
	mrf.recv_car_data(&cardata);
	Serial.print("Tilt: ");
	Serial.println(cardata.tilt.tilt_degrees);
	Serial.print("Battery %: ");
	Serial.println(cardata.battery_percentage);
	#endif
}

void handle_tx() {
	if(!mrf.get_txinfo()->tx_ok)
	Serial.println("TX failed!");

}
