#define CAR

#include <stdbool.h>
#include <stdint.h>

#include <Wire.h>
#include <Servo.h>
#include <SPI.h>

//#include "mrf24j.h"
#include "comm.h"

//controls
#include "car_devices/SteeringControl.h"
#include "car_devices/MotorControl.h"
//sensors
#include "car_devices/ParkingSensors.h"
#include "car_devices/TiltAlarm.h"
#include "car_devices/Lights.h"

#define CONTROLLER_ADDRESS 0x6000

//Mrf24j mrf(/*pin reset*/ 4, /*pin CS*/ 9, /*pin itnerrupt*/ 3);
CarData cardata;
CtrlData ctrldata;
const unsigned int SYNC_INTERVAL_MS = 100;
unsigned long sync_last_time = 0;

bool rxl = 0;

#include <SPI.h>

class ESPSafeMaster
{
	private:
	uint8_t _ss_pin;
	void _pulseSS()
	{
		digitalWrite(_ss_pin, HIGH);
		delayMicroseconds(5);
		digitalWrite(_ss_pin, LOW);
	}
	public:
	ESPSafeMaster(uint8_t pin):_ss_pin(pin) {}
	void begin()
	{
		pinMode(_ss_pin, OUTPUT);
		_pulseSS();
	}

	uint32_t readStatus()
	{
		_pulseSS();
		SPI.transfer(0x04);
		uint32_t status = (SPI.transfer(0) | ((uint32_t)(SPI.transfer(0)) << 8) | ((uint32_t)(SPI.transfer(0)) << 16) | ((uint32_t)(SPI.transfer(0)) << 24));
		_pulseSS();
		return status;
	}

	void writeStatus(uint32_t status)
	{
		_pulseSS();
		SPI.transfer(0x01);
		SPI.transfer(status & 0xFF);
		SPI.transfer((status >> 8) & 0xFF);
		SPI.transfer((status >> 16) & 0xFF);
		SPI.transfer((status >> 24) & 0xFF);
		_pulseSS();
	}

	void readData(uint8_t * data)
	{
		_pulseSS();
		SPI.transfer(0x03);
		SPI.transfer(0x00);
		for(uint8_t i=0; i<32; i++) {
			data[i] = SPI.transfer(0);
		}
		_pulseSS();
	}

	void writeData(uint8_t * data, size_t len)
	{
		uint8_t i=0;
		_pulseSS();
		SPI.transfer(0x02);
		SPI.transfer(0x00);
		while(len-- && i < 32) {
			SPI.transfer(data[i++]);
		}
		while(i++ < 32) {
			SPI.transfer(0);
		}
		_pulseSS();
	}

	String readData()
	{
		char data[33];
		data[32] = 0;
		readData((uint8_t *)data);
		return String(data);
	}

	void writeData(const char * data)
	{
		writeData((uint8_t *)data, strlen(data));
	}
};

ESPSafeMaster esp(9);

void setup()
{
	//setup_mrf(0x6001, 0xcafe);

	MotorControl.init();
	//Lights.init(50);

	//ParkingSensors.init();
	//SteeringControl.init();
	pinMode(1, OUTPUT);
	digitalWrite(1, LOW);
	//TiltAlarm.init();
	//sei();
	SPI.begin();
	esp.begin();
	delay(1000);
}
void send(const char * message)
{
	esp.writeData(message);
	delay(10);
	esp.readData();
}
void loop()
{
	unsigned long current_time = millis();
	
	//check if a new message came and update CarData and CtrlData if necessary
	//mrf.check_flags(&mrf_rx, &mrf_tx);

	//use them data
	MotorControl.loop(ctrldata, current_time);
	//SteeringControl.steer(ctrldata.steering);
	//TiltAlarm.loop(); errore
	//ParkingSensors.loop();
	//Lights.loop();


	if(current_time - sync_last_time >= SYNC_INTERVAL_MS)
	{
		int32_t status = esp.readStatus();
		int8_t steer = status & 0xff;
		int8_t throttle = (status >> 8) & 0xff;
		ctrldata.throttle = throttle;
		ctrldata.steering = steer;
		//debug stuff
		
	/*	mrf.read_rxdata();
		mrf.recv_ctrl_data(&ctrldata);
		
		
		mrf.start_tx(CONTROLLER_ADDRESS, sizeof(cardata));
		mrf.send_car_data(&cardata);
		mrf.finish_tx();*/
		
		update_cardata();

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

/*void setup_mrf(word address, word pan)
{
	mrf.reset();
	mrf.init();
	mrf.set_pan(pan);
	mrf.address16_write(address);
	mrf.set_palna(true);
	mrf.set_promiscuous(true);
	//attachInterrupt(1, mrf_isr, CHANGE); // interrupt 1 equivalent to pin 3(INT1) on ATmega8/168/328
}*/

void update_cardata()
{
	//TODO: zmerat baterku
	cardata.battery_percentage = -1;

	//Lights.update_cardata(cardata);
	//SteeringControl.update_cardata(cardata); ??? preco?
	//TiltAlarm.update_cardata(cardata);
	//ParkingSensors.update_cardata(cardata);
}