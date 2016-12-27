#pragma once

#define LI_SENSOR_PIN 0 // light sendor pin number (analog)
#define LI_FRONT_PIN 1 // front headlights pin number (analog)
#define LI_TRESHOLD lightTreshold // turn on lights below this value
#define LI_CYCLE_CHANGE 16 // change headlights brightness by this value every cycle
#define LI_DAYLIGHT 255 // normal headlights brightness
#define LI_NIGHTLIGHT 1023 // night headlights brightness


/// <summary>
/// automatic heandlights control
/// </summary>
/// <example>
/// Example implementation
/// <code>
/// #include "Lights.h"
/// void setup() {
///		Lights::init();
/// }
/// void loop() {
///		Lights::cycle();
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
	static void init() {
		init(256);
	}

public:
	/// <summary>
	/// initiates Lights with custom treshold
	/// </summary>
	/// <param name='treshold'>headlights will turn on below this value</param>
	static void init(int treshold) {
		lightTreshold = treshold;
		lastCheck = false;
		lightState = LI_DAYLIGHT;
	}

public:
	/// <summary>
	/// updates treshold value at runtime
	/// </summary>
	/// <param name='treshold'>headlights will turn on below this value</param>
	static void updateTreshold(int treshold) {
		lightTreshold = treshold;
	}

public:
	/// <summary>
	/// executes one check cycle
	/// </summary>
	static void cycle() {
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

public:
	/// <summary>
	/// checks if current light level is below treshold value
	/// </summary>
	/// <returns>true if condition applies</returns>
	static bool dark() {
		return analogRead(LI_SENSOR_PIN) > LI_TRESHOLD; // returns true if light level is below treshold value
	}

	// make constructor private
private:
	Lights() {}
};