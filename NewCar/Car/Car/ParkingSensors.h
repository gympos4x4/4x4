// ParkingSensors.h
// Author: Juraj Marcin

#ifndef _PARKINGSENSORS_h
#define _PARKINGSENSORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define PS_SENSOR_COUNT 8
//#define PS_BEEP_PIN 13
#define PS_BEEP_STATE_HIGH HIGH
#define PS_BEEP_STATE_LOW LOW
#define PS_BEEP_LENGHT 250

#include "eADC.h"

/// <summary>
/// communication with external ADC (MCP3208)
/// </summary>
/// <example>
/// Example implementation
/// <code>
/// #include "ParkingSensors.h"
/// void setup() {
/// 	ParkingSensors::init();
/// }
/// void loop() {
/// 	ParkingSensors::loop();
/// }
/// </code>
/// </example>
class ParkingSensors {

	static uint16_t calibratedMin;
	static uint16_t calibratedMax;
	static uint16_t calibratedTreshold;
	static bool beeping;
	static uint64_t milisstart;

public:
	/// <summary>
	/// initiates parking sensors
	/// </summary>
	static void init();

public:
	/// <summary>
	/// calibrates maximum distance
	/// </summary>
	/// <param name='sensor'>sensor to use for calibration</param>
	static void calibrateMax(int sensor = 0);

public:
	/// <summary>
	/// calibrates minimum distance
	/// </summary>
	/// <param name='sensor'>sensor to use for calibration</param>
	static void calibrateMin(int sensor = 0);

public:
	/// <summary>
	/// calibrates beeping treshold
	/// </summary>
	/// <param name='sensor'>sensor to use for calibration</param>
	static void calibrateTreshold(int sensor = 0);

public:
	/// <summary>
	/// reads sensor value
	/// </summary>
	/// <param name='sensor'>sensor to read from</param>
	static uint16_t readsensor(int sensor);

public:
	/// <summary>
	/// executes one check cycle
	/// </summary>
	static void loop();

	// make constructor private
private:
	ParkingSensors();
};

#endif

