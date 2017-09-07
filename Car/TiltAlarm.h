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
	static void init();

	/// <summary>
	/// initiates TiltAlarm with custom angle treshold
	/// </summary>
	/// <param name='angle'>custom angle treshold 0-90</param>
	static void init(byte angle);

	/// <summary>
	/// updates angle treshold value at runtime
	/// </summary>
	/// <param name='angle'>custom angle treshold 0-90</param>
	static void updateAngle(byte angle);

	/// <summary>
	/// executes one check cycle
	/// </summary>
	static void cycle();

	/// <summary>
	/// checks if current filter values are mostly tilted
	/// </summary>
	/// <returns>true if condition applies</returns>
	static bool tilted();

	/// <summary>
	/// executes one check 
	/// </summary>
	static void nextCheck();

private:
	/// <summary>
	/// get current angle in x axis
	/// </summary>
	/// <returns>returns angle in degrees</returns>
	static int getAngle(int x, int z);

	/// <summary>
	/// changes indicators state
	/// </summary>
	/// <param name='value'>indicators state</param>
	static void signals(bool value);

	// make constructor private
private:
	TiltAlarm() {}
};