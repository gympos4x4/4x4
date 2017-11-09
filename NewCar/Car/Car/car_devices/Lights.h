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
#define LI_FRONT_PIN 1 // front headlights pin number (PWM)
#define LI_TRESHOLD lightTreshold // turn on lights below this value
#define LI_CYCLE_CHANGE 16 // change headlights brightness by this value every cycle
#define LI_DAYLIGHT 255 // normal headlights brightness
#define LI_NIGHTLIGHT 1023 // night headlights brightness

/// <summary>
/// automatic headlights control
/// </summary>
/// <example>
/// Example implementation
/// <code>
/// #include "Lights.h"
/// void setup() {
///		Lights::init();
/// }
/// void loop() {
///		Lights::loop();
/// }
/// </code>
/// </example>
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
	/// initiates Lights with custom treshold
	/// </summary>
	/// <param name='treshold'>headlights will turn on below this value</param>
	void init(int treshold);

	/// <summary>
	/// updates treshold value at runtime
	/// </summary>
	/// <param name='treshold'>headlights will turn on below this value</param>
	void update_treshold(int treshold);

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

