#define CAR

#include <stdbool.h>
#include <stdint.h>

#include <Wire.h>
#include <Servo.h>
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

#define CONTROLLER_ADDRESS 0x6000

Mrf24j mrf(/*pin reset*/ 4, /*pin CS*/ 9, /*pin itnerrupt*/ 3);
CarData cardata;
CtrlData ctrldata;
const unsigned int SYNC_INTERVAL_MS = 100;
unsigned long sync_last_time = 0;

bool rxl = 0;
void setup()
{
	setup_mrf(0x6001, 0xcafe);

	//MotorControl.init();
	//Lights.init(50);

	//ParkingSensors.init();
	//SteeringControl.init();
	pinMode(1, OUTPUT);
	digitalWrite(1, HIGH);
	TiltAlarm.init();
	//sei();
}

void loop()
{
	unsigned long current_time = millis();
	
	//check if a new message came and update CarData and CtrlData if necessary
	//mrf.check_flags(&mrf_rx, &mrf_tx);

	//use them data
	//MotorControl.loop(ctrldata, current_time);
	//SteeringControl.steer(ctrldata.steering);
	//TiltAlarm.loop(); errore
	//ParkingSensors.loop();
	//Lights.loop();


	if(current_time - sync_last_time >= SYNC_INTERVAL_MS)
	{
		//update_cardata();
		
		//debug stuff
		cardata.throttleFb = ctrldata.throttle;
		cardata.steerFb = ctrldata.steering;
		cardata.battery_percentage = 102;
		
		mrf.read_rxdata();
		mrf.recv_ctrl_data(&ctrldata);
		
		
		mrf.start_tx(CONTROLLER_ADDRESS, sizeof(cardata));
		mrf.send_car_data(&cardata);
		mrf.finish_tx();
		
		sync_last_time = current_time;
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
	//digitalWrite(10,rxl);		debug statement?
}

void mrf_isr()
{
	//mrf.interrupt_handler(); this shouldn't happen
}
void mrf_rx()
{
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
	mrf.set_promiscuous(true);
	//attachInterrupt(1, mrf_isr, CHANGE); // interrupt 1 equivalent to pin 3(INT1) on ATmega8/168/328
}

void update_cardata()
{
	//TODO: zmerat baterku
	cardata.battery_percentage = -1;

	//Lights.update_cardata(cardata);
	//SteeringControl.update_cardata(cardata); ??? preco?
	TiltAlarm.update_cardata(cardata);
	//ParkingSensors.update_cardata(cardata);
}