// Lights.h
// Author: Juraj Marcin

#ifndef _LIGHTS_h
#define _LIGHTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define LI_SENSOR_PIN 0 // light sensor pin number (analog) TODO: update pin
#define LI_FRONT_PIN 10 // front headlights pin number (PWM)
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
class Lights {

	static int lightTreshold;
	static bool lastCheck;
	static int lightState;

public:
	/// <summary>
	/// initiates Lights with default treshold
	/// </summary>
	static void init();

public:
	/// <summary>
	/// initiates Lights with custom treshold
	/// </summary>
	/// <param name='treshold'>headlights will turn on below this value</param>
	static void init(int treshold);

public:
	/// <summary>
	/// updates treshold value at runtime
	/// </summary>
	/// <param name='treshold'>headlights will turn on below this value</param>
	static void updateTreshold(int treshold);

public:
	/// <summary>
	/// executes one check cycle
	/// </summary>
	static void loop();

public:
	/// <summary>
	/// checks if current light level is below treshold value
	/// </summary>
	/// <returns>true if condition applies</returns>
	static bool dark();

	// make constructor private
private:
	Lights();
};

#endif

