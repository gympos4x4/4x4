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


void setup()
{
	//setup_mrf(0x6001, 0xcafe);
	TiltAlarm.init();
	Lights.init();
	//ParkingSensors.init();
	//SteeringControl.init();
	//sei();
	//SPI.begin();
	//esp.begin();
}
bool light;
void loop()
{
	//digitalWrite(1, analogRead(A0) < 102 && light);
	//light = !light;
	Lights.loop();
	TiltAlarm.loop();
	delay(5);
	return;
	unsigned long current_time = millis();
	
	//check if a new message came and update CarData and CtrlData if necessary
	//mrf.check_flags(&mrf_rx, &mrf_tx);

	//use them data
	//MotorControl.loop(ctrldata, current_time);
	//SteeringControl.steer(ctrldata.steering);
	TiltAlarm.loop();
	//ParkingSensors.loop();
	Lights.loop();
}
