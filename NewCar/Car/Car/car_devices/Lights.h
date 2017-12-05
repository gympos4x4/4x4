// Lights.h
// Author: Juraj Marcin

#ifndef _LIGHTS_h
#define _LIGHTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "MCP23008.h"

#define LI_SENSOR_PIN A0
#define LI_FRONT_PIN 7
#define LI_REAR_PIN 6
#define LI_REVERSE_PIN 5
#define LI_TRESHOLD 36 // turn on lights below this value

class _Lights {

	int lightTreshold;
	bool lastCheck;
	int lightState;
	MCP23008 mcp;

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

	void set_front(bool state);
	void set_rear(bool state);
	void set_side(bool state);
	/// <summary>
	/// checks if current light level is below treshold value
	/// </summary>
	/// <returns>true if condition applies</returns>
	bool is_below_threshold();

	void update_cardata(class CarData& cardata);
};

extern _Lights Lights;

#endif

