// Author: Juraj Maarcin

#include "Lights.h"

void Lights::init() {
	init(256);
}

void Lights::init(int treshold) {
	lightTreshold = treshold;
	lastCheck = false;
	lightState = LI_DAYLIGHT;
}

void Lights::updateTreshold(int treshold) {
	lightTreshold = treshold;
}

void Lights::loop() {
	lastCheck = dark();
	if (lastCheck) { // if light level is below treshold value headlights brighness increases every cycle by LI_CYCLE_CHANGE until it hits max value
		if (lightState + LI_CYCLE_CHANGE <= LI_NIGHTLIGHT)
			lightState += LI_CYCLE_CHANGE;
	} else { // otherwise it decreases by same value until it hist its min value
		if (lightState - LI_CYCLE_CHANGE >= LI_DAYLIGHT)
			lightState -= LI_CYCLE_CHANGE;
	}
	analogWrite(LI_FRONT_PIN, lightState); // writes current brightness value to headlight pin
}

bool Lights::dark() {
	return analogRead(LI_SENSOR_PIN) > LI_TRESHOLD; // returns true if light level is below treshold value
}
