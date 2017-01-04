/*
 Name:		Car.ino
 Created:	1/4/17 2:35:04 AM
 Author:	Matej
*/

// the setup function runs once when you press reset or power the board
#include "SharedLib.h"
#include "Lights.h"
#include "TiltAlarm.h"

void setup() {
	RadioConfig rc;
	
	auto sm = new SyncManager(0, nullptr, rc);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
