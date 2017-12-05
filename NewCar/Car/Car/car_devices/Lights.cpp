// Author: Juraj Maarcin

#include "Lights.h"
#include "../comm.h"

_Lights Lights;

void _Lights::init() {
	mcp.begin();
	mcp.pinMode(LI_FRONT_PIN, OUTPUT);
	mcp.pinMode(LI_REAR_PIN, OUTPUT);
	mcp.pinMode(LI_REVERSE_PIN, OUTPUT);
}

void _Lights::loop() {
	bool below = is_below_threshold();
	mcp.digitalWrite(LI_FRONT_PIN, below);
	mcp.digitalWrite(LI_REAR_PIN, below);
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
