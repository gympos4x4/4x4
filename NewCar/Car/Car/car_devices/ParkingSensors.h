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
#define PS_IR_PIN 8

#include "eADC.h"

class _ParkingSensors {
	public:
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
	/// reads sensor value
	/// </summary>
	/// <param name='sensor'>sensor to read from</param>
	int16_t readsensor(int sensor);

	/// <summary>
	/// executes one check cycle
	/// </summary>
	void loop();

	/// <summary>
	/// updates data that's being sent to the controller
	/// </summary>
	/// <param name='cardata'>CarData instance</param>
	void update_cardata(class CarData& cardata);
};

extern _ParkingSensors ParkingSensors;

#endif

