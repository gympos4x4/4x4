// Lights.h
// Author: Juraj Marcin

#ifndef _LIGHTS_h
#define _LIGHTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define LI_SENSOR_PIN A0 // light sensor pin number (analog) TODO: update pin
#define LI_LIGHTS_PIN 1 // front headlights pin number (PWM)
#define LI_TRESHOLD 36 // turn on lights below this value

class _Lights {

	int lightTreshold;
	bool lastCheck;
	int lightState;

public:
	_Lights() {}
	~_Lights() {}

public:
	/// <summary>
	/// initiates Lights with default treshold
	/// </summary>
	void init();

	/// <summary>
	/// executes one check cycle
	/// </summary>
	void loop();

	/// <summary>
	/// checks if current light level is below treshold value
	/// </summary>
	/// <returns>true if condition applies</returns>
	bool is_below_threshold();

	void update_cardata(class CarData& cardata);
};

extern _Lights Lights;

#endif

