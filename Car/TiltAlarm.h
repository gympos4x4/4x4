#pragma once

#define TA_CHECK_COUNT 100
#define TA_ALERT_TRESHOLD 10 // treshold as percentage; enable: value, disable: 100-value
#define TA_ALERT_ANGLE alertAngle // minimal alert angle (value;infinite)
#define TA_MPU_ADDR 0x68 // MPU-6050 #1 IC2 address
#define TA_PWR_MGMT_1 0x6B // PWR_MGMT_1 register
#define TA_LED_PIN 13 // alert led pin
#define TA_LED_BLINK_INTERVAL 500 // alert led interval [ms]

#include <Wire.h>

/// <summary>
/// alert based on current tilt angle
/// </summary>
/// <example>
/// Example implementation
/// <code>
/// #include "Wire.h"
/// #include "TiltAlarm.h"
/// void setup() {
///		TiltAlarm::init(); // or TiltAlarm::init(20); for custom angle
/// }
/// void loop() {
/// 	TiltAlarm::cycle();
/// }
/// </code>
/// </example>
class TiltAlarm {

	static int acX, acZ;
	static bool tiltChecks[TA_CHECK_COUNT];
	static unsigned char checkCount;
	static bool alreadyTilted;
	static unsigned long lastCycle;
	static bool ledOn;
	static bool initDone;
	static byte alertAngle;

public:
	/// <summary>
	/// initiates TiltAlarm with default angle treshold
	/// </summary>
	static void init() {
		init(20);
	}
	
public:
	/// <summary>
	/// initiates TiltAlarm with custom angle treshold
	/// </summary>
	/// <param name='angle'>custom angle treshold 0-90</param>
	static void init(byte angle) {
		alertAngle = angle;
		checkCount = 0;
		alreadyTilted = false;
		ledOn = false;
		Wire.begin();
		Wire.beginTransmission(TA_MPU_ADDR);
		Wire.write(TA_PWR_MGMT_1);
		Wire.write(0); // wake up th MPU
		Wire.endTransmission(true);
		pinMode(TA_LED_PIN, OUTPUT); // set pinmode for alert led
		initDone = true;
	}

public:
	/// <summary>
	/// updates angle treshold value at runtime
	/// </summary>
	/// <param name='angle'>custom angle treshold 0-90</param>
	static void updateAngle(byte angle) {
		alertAngle = angle;
	}

public:
	/// <summary>
	/// executes one check cycle
	/// </summary>
	static void cycle() {
		nextCheck();
		if (tilted()) { // check tilt with tresholds
			if (millis() >= lastCycle + TA_LED_BLINK_INTERVAL) {
				signals(!ledOn);
				ledOn = !ledOn;
			}
		} else {
			signals(false);
		}
		lastCycle = millis();
	}

public:
	/// <summary>
	/// checks if current filter values are mostly tilted
	/// </summary>
	/// <returns>true if condition applies</returns>
	static bool tilted() {
		unsigned char trueChecks = 0;
		for (unsigned char i = 0; i < TA_CHECK_COUNT; i++)
			if (tiltChecks[i])
				trueChecks++; // counts how many checks are true
		if (alreadyTilted) {
			// if car is already tilted then checks if true checks count is over alert disable treshold
			alreadyTilted = trueChecks > TA_CHECK_COUNT * (100 - TA_ALERT_TRESHOLD) / 100;
		} else {
			// if car is't tilted then checks if true checks count is over alert enable treshold
			alreadyTilted = trueChecks > TA_CHECK_COUNT * TA_ALERT_TRESHOLD / 100;
		}
		return alreadyTilted;
	}

public:
	/// <summary>
	/// executes one check 
	/// </summary>
	static void nextCheck() {
		if (!initDone)
			return; // return if init failed or hasn't been ran
		/*
			GET VALUES FROM SENSOR
		*/
		Wire.beginTransmission(TA_MPU_ADDR);
		Wire.write(0x3B); // start with index 0x3B (x axis [ACCEL_XOUT_H])
		Wire.endTransmission(false);
		Wire.requestFrom(TA_MPU_ADDR, 6, true); // request 6 registers <0x3B;0x40> [ACCEL_XOUT_H, ACCEL_XOUT_L, ACCEL_YOUT_H, ACCEL_YOUT_L, ACCEL_ZOUT_H, ACCEL_ZOUT_L]
		acX = Wire.read() << 8 | Wire.read(); // read 0x3B [ACCEL_XOUT_H], 0x3C [ACCEL_XOUT_L]
		Wire.read() << 8 | Wire.read(); // skip 0x3D [ACCEL_YOUT_H], 0x3E [ACCEL_YOUT_L]
		acZ = Wire.read() << 8 | Wire.read(); // read 0x3F [ACCEL_ZOUT_H], 0x40 [ACCEL_ZOUT_L]
		/*
			CHECKS CAR ANGLE
		*/
		if (checkCount < TA_CHECK_COUNT) {
			tiltChecks[checkCount] = getAngle(acX, acZ) > TA_ALERT_ANGLE;
			checkCount++;
		} else {
			checkCount = 0;
		}
	}

private:
	/// <summary>
	/// get current angle in x axis
	/// </summary>
	/// <returns>returns angle in degrees</returns>
	static int getAngle(int x, int z) {
		return abs(degrees(atan2(z, x)));
	}

private:
	/// <summary>
	/// changes indicators state
	/// </summary>
	/// <param name='value'>indicators state</param>
	static int signals(bool value) {
		if (value) {
			digitalWrite(TA_LED_PIN, HIGH);
		} else {
			digitalWrite(TA_LED_PIN, LOW);
		}
	}

	// make constructor private
private:
	TiltAlarm() {}
};