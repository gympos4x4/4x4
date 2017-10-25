// Author: Juraj Maarcin

#include "Lights.h"
#include "../comm.h"

_Lights Lights;

void _Lights::init() {
	init(256);
}

void _Lights::init(int treshold) {
	lightTreshold = treshold;
	lastCheck = false;
	lightState = LI_DAYLIGHT;
}

void _Lights::update_treshold(int treshold) {
	lightTreshold = treshold;
}

void _Lights::loop() {
	lastCheck = is_below_threshold();
	if (lastCheck) { // if light level is below treshold value headlights brighness increases every cycle by LI_CYCLE_CHANGE until it hits max value
		if (lightState + LI_CYCLE_CHANGE <= LI_NIGHTLIGHT)
			lightState += LI_CYCLE_CHANGE;
	} else { // otherwise it decreases by same value until it hist its min value
		if (lightState - LI_CYCLE_CHANGE >= LI_DAYLIGHT)
			lightState -= LI_CYCLE_CHANGE;
	}
	analogWrite(LI_FRONT_PIN, lightState); // writes current brightness value to headlight pin
}

bool _Lights::is_below_threshold() {
	return analogRead(LI_SENSOR_PIN) > LI_TRESHOLD; // returns true if light level is below treshold value
}

void _Lights::update_cardata(class CarData& cardata)
{
	cardata.lights.is_below_threshold = is_below_threshold();
	cardata.lights.state = lightState;
	cardata.lights.threshold = lightTreshold;
}
