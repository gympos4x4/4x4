#pragma once

#define PS_SENSOR_COUNT 1
#define PS_BEEP_PIN 13
#define PS_BEEP_STATE_HIGH HIGH
#define PS_BEEP_STATE_LOW LOW
#define PS_BEEP_LENGHT 250

#include <eADC.h>

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
/// 	ParkingSensors::cycle();
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
	static void init() {
		calibratedMin = 0;
		calibratedMax = 4096;
		calibratedTreshold = 3072;
		beeping = false;
		pinMode(PS_BEEP_PIN, OUTPUT);
		eADC::init();
	}

public:
	/// <summary>
	/// calibrates maximum distance
	/// </summary>
	/// <param name='sensor'>sensor to use for calibration</param>
	static void calibrateMax(int sensor = 0) {
		calibratedMax = eADC::analogRead(sensor);
	}

public:
	/// <summary>
	/// calibrates minimum distance
	/// </summary>
	/// <param name='sensor'>sensor to use for calibration</param>
	static void calibrateMin(int sensor = 0) {
		calibratedMin = eADC::analogRead(sensor);
	}

public:
	/// <summary>
	/// calibrates beeping treshold
	/// </summary>
	/// <param name='sensor'>sensor to use for calibration</param>
	static void calibrateTreshold(int sensor = 0) {
		calibratedTreshold = eADC::analogRead(sensor);
	}

public:
	/// <summary>
	/// reads sensor value
	/// </summary>
	/// <param name='sensor'>sensor to read from</param>
	static uint16_t readsensor(int sensor) {
		if (PS_SENSOR_COUNT > sensor) {
			return map(eADC::analogRead(sensor), 0, 4096, calibratedMin, calibratedMax);
		}
		return 0;
	}

public:
	/// <summary>
	/// executes one check cycle
	/// </summary>
	static void cycle() {
		uint16_t highest = 4096;
		for (int i = 0; i < PS_SENSOR_COUNT; i++) {
			uint16_t current = readsensor(i);
			if (current > highest) {
				highest = current;
			}
		}
		if (highest > calibratedTreshold) {
			beeping = false;
		}
		else
		{
			if (beeping) {
				if (milisstart + PS_BEEP_LENGHT <= millis()) {
					beeping = false;
					milisstart = millis();
				}
			}
			else {
				if (milisstart + highest <= millis()) {
					beeping = true;
					milisstart = millis();
				}
			}
		}
		if (beeping) {
			digitalWrite(PS_BEEP_PIN, PS_BEEP_STATE_HIGH);
		}
		else {
			digitalWrite(PS_BEEP_PIN, PS_BEEP_STATE_LOW);
		}
	}

	// make constructor private
private:
	ParkingSensors() {}
};