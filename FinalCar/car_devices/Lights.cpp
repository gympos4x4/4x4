// Author: Juraj Maarcin

#include "Lights.h"
#include "../comm.h"

_Lights Lights;

void _Lights::init() {
	pinMode(LI_LIGHTS_PIN, OUTPUT);
}

void _Lights::loop() {
	//lastCheck = is_below_threshold();
	digitalWrite(LI_LIGHTS_PIN, analogRead(LI_SENSOR_PIN) < LI_TRESHOLD);
}

bool _Lights::is_below_threshold() {
	return analogRead(LI_SENSOR_PIN) < LI_TRESHOLD; // returns true if light level is below treshold value
}

void _Lights::update_cardata(class CarData& cardata)
{
	cardata.lights.is_below_threshold = is_below_threshold();
	cardata.lights.state = lightState;
	cardata.lights.threshold = lightTreshold;
}
