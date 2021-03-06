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
#define PS_IR_PIN 7
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
class _ParkingSensors {

	int16_t calibratedMin;
	int16_t calibratedMax;
	int16_t calibratedThreshold;
	bool beeping;
	uint64_t milisstart;
	int16_t sensorData[PS_SENSOR_COUNT];

public:
	_ParkingSensors() {}
	~_ParkingSensors() {}

public:
	/// <summary>
	/// initiates parking sensors
	/// </summary>
	void init();

	/// <summary>
	/// calibrates maximum distance
	/// </summary>
	/// <param name='sensor'>sensor to use for calibration</param>
	void calibrateMax(int sensor);

	void calibrateMax() { calibrateMax(0); }

	/// <summary>
	/// calibrates minimum distance
	/// </summary>
	/// <param name='sensor'>sensor to use for calibration</param>
	void calibrateMin(int sensor);
	void calibrateMin() { calibrateMin(0); }

	/// <summary>
	/// calibrates beeping threshold
	/// </summary>
	/// <param name='sensor'>sensor to use for calibration</param>
	void calibrateThreshold(int sensor);
	void calibrateThreshold() { calibrateThreshold(0); }

	/// <summary>
	/// reads sensor value
	/// </summary>
	/// <param name='sensor'>sensor to read from</param>
	int16_t readsensor(int sensor);

	/// <summary>
	/// executes one check cycle
	/// </summary>
	void loop();

	void update_cardata(class CarData& cardata);
};

extern _ParkingSensors ParkingSensors;

#endif

