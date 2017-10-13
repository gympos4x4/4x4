/*
 Name:		Car.ino
 Created:	1/4/17 2:35:04 AM
 Author:	Matej
*/

//shared library includes

//define these in project options
//#define TX_SYNC_COUNT 5
//#define RX_SYNC_COUNT 5
#include "SharedLib.h"

//local library includes
/*
#include "eADC.h"
#include "Lights.h"
#include "MotorControl.h"
#include "ParkingSensors.h"
#include "SteeringControl.h"
#include "TiltAlarm.h"
*/

RadioConfig rc = mega_radio_config;
Mrf24j mrf(rc.pin_reset, rc.pin_cs, rc.pin_interrupt);

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	rc.buddy_address = uno_radio_config.my_address;

	SyncManager::setup(&rc, &mrf, nullptr, 0, nullptr, 0);
}

// the loop function runs over and over again until power down or reset
void loop() {
	//do everything that's needed
	SyncManager::loop();
}
