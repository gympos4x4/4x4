#define CAR
#define F_CPU 16000000UL
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
#define SPKR_PIN TA_ALERT_PIN

Mrf24j mrf(/*pin reset*/ 4, /*pin CS*/ 9, /*pin itnerrupt*/ 2);

CarData cardata;
CtrlData ctrldata;
const unsigned int SYNC_INTERVAL_MS = 10;
unsigned long sync_last_time = 0;

#include <SPI.h>

MCP23008 mcp;
bool shouldSendData = true;

void setup()
{
	pinMode(SPKR_PIN, OUTPUT);
	setup_mrf(0x6001, 0xcafe);
	//MotorControl.init();
	Lights.init();

	ParkingSensors.init();
	//SteeringControl.init();
	TiltAlarm.init();
	Serial.begin(9600);
	delay(1000);
}
void loop()
{
	unsigned long current_time = millis();
	
	//check if a new message came and update CarData and CtrlData if necessary
	//mrf.check_flags(&mrf_rx, &mrf_tx);

	//use them data
	//MotorControl.loop(ctrldata);
	//SteeringControl.steer(ctrldata.steering);
	TiltAlarm.loop();
	ParkingSensors.loop();
	Lights.loop();
	

	if(current_time - sync_last_time >= SYNC_INTERVAL_MS)
	{
		/*if(mrf.get_pan() == 0xcafe)
		Serial.println("PAN ok");*/
		/*if(cardata.battery_percentage >= 100)
		cardata.battery_percentage = 0;
		else
		cardata.battery_percentage++;*/
		//Serial.println((int)ctrldata.throttle);
		//debug stuff
		update_cardata();
		
		mrf.start_tx(CONTROLLER_ADDRESS, sizeof(cardata));
		mrf.send_car_data(&cardata);
		mrf.finish_tx();

		sync_last_time = current_time;
	}
	//digitalWrite(10,rxl);		debug statement?
}

void mrf_isr()
{
	/*cli();
	Serial.println("Int0 Triggered");
	//mrf.interrupt_handler();// this shouldn't happen
	Serial.print("INTSTAT: ");
	Serial.println(mrf.read_short(MRF_INTSTAT));
	sei();*/
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
/*	mcp.begin();
	mcp.pinMode(4, INPUT);
	mcp.pullUp(4, HIGH);
	if (mcp.digitalRead(4)) {
		mrf.set_channel(17);
		for (int i = 0; i < 10; i++)
		{
			digitalWrite(SPKR_PIN, HIGH);
			delay(100);
			digitalWrite(SPKR_PIN, LOW);
			delay(100);
		}
	}*/
	mrf.set_pan(pan);
	mrf.address16_write(address);
	mrf.set_palna(true);
	mrf.set_promiscuous(true);
	//attachInterrupt(1, mrf_isr, CHANGE); // interrupt 1 equivalent to pin 3(INT1) on ATmega8/168/328
}

void update_cardata()
{
	//TODO: zmerat baterku
	//cardata.battery_percentage = -1;
	cardata.battery_percentage = (analogRead(A1) - 614) / 2;       //100 = 840, 0 = 614

	Lights.update_cardata(cardata);
	//SteeringControl.update_cardata(cardata); //??? preco?
	TiltAlarm.update_cardata(cardata);
	ParkingSensors.update_cardata(cardata);
}