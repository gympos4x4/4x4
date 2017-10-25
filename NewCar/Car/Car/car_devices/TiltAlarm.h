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

class CarData;

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
class _TiltAlarm {

	int8_t acX, acZ;
	bool tiltChecks[TA_CHECK_COUNT];
	uint8_t checkCount;
	bool alreadyTilted;
	uint64_t lastCycle;
	bool ledOn;
	bool initDone;
	uint8_t alertAngle;

public:
	_TiltAlarm() = default;
	~_TiltAlarm()= default;

public:
	/// <summary>
	/// initiates TiltAlarm with default angle threshold
	/// </summary>
	void init();

	/// <summary>
	/// initiates TiltAlarm with custom angle threshold
	/// </summary>
	/// <param name='angle'>custom angle treshold 0-90</param>
	void init(uint8_t angle);

	/// <summary>
	/// updates angle threshold value at runtime
	/// </summary>
	/// <param name='angle'>custom angle treshold 0-90</param>
	void updateAngle(uint8_t angle);

	/// <summary>
	/// executes one check cycle
	/// </summary>
	void loop();

	/// <summary>
	/// checks if current filter values are mostly tilted
	/// </summary>
	/// <returns>true if condition applies</returns>
	bool tilted();

	/// <summary>
	/// executes one check 
	/// </summary>
	void nextCheck();

	void update_cardata(CarData& cardata);

private:
	/// <summary>
	/// get current angle in x axis
	/// </summary>
	/// <returns>returns angle in degrees</returns>
	int getAngle(int x, int z);

	/// <summary>
	/// changes indicators state
	/// </summary>
	/// <param name='value'>indicators state</param>
	void signals(bool value);
};

extern _TiltAlarm TiltAlarm;

#endif

