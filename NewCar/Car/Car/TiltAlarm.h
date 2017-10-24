// TiltAlarm.h
// Author: Juraj Marcin

#ifndef _TILTALARM_h
#define _TILTALARM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define TA_CHECK_COUNT 100
#define TA_ALERT_TRESHOLD 10 // treshold as percentage; enable: value, disable: 100-value
#define TA_ALERT_ANGLE alertAngle // minimal alert angle (value;infinite)
#define TA_MPU_ADDR 0x68 // MPU-6050 #1 IC2 address
#define TA_PWR_MGMT_1 0x6B // PWR_MGMT_1 register
#define TA_LED_PIN 13 // alert led pin TODO: update pin
#define TA_LED_BLINK_INTERVAL 500 // alert led interval [ms]

#include <Wire.h>
#include <stdint.h>

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

	static int8_t acX, acZ;
	static bool tiltChecks[TA_CHECK_COUNT];
	static uint8_t checkCount;
	static bool alreadyTilted;
	static uint64_t lastCycle;
	static bool ledOn;
	static bool initDone;
	static uint8_t alertAngle;

public:
	/// <summary>
	/// initiates TiltAlarm with default angle threshold
	/// </summary>
	static void init();

	/// <summary>
	/// initiates TiltAlarm with custom angle threshold
	/// </summary>
	/// <param name='angle'>custom angle treshold 0-90</param>
	static void init(uint8_t angle);

	/// <summary>
	/// updates angle threshold value at runtime
	/// </summary>
	/// <param name='angle'>custom angle treshold 0-90</param>
	static void updateAngle(uint8_t angle);

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
	TiltAlarm();
};

#endif

