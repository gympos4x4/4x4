// TiltAlarm.h
// Author: Juraj Marcin

#ifndef _TILTALARM_h
#define _TILTALARM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define TA_ALERT_TRESHOLD 100 // treshold as percentage; enable: value, disable: 100-value
#define TA_ALERT_ANGLE 25 // minimal alert angle (value;infinite)
#define TA_MPU_ADDR 0x68 // MPU-6050 #1 IC2 address
#define TA_PWR_MGMT_1 0x6B
#define TA_ALERT_PIN 8
//#define TA_PIEZO_PIN 8
#define TA_ALERT_INTERVAL 250 // alert led interval [ms]
#define TA_ALERT_ENABLED HIGH

#include <Wire.h>
#include <stdint.h>

class CarData;
class _TiltAlarm {
	
	public:
	int16_t AcX, AcY, AcZ;
	int rotZ;
	uint64_t lastCheck;
	bool alertON;
	bool isTilted;
	bool alreadyTilted;
	uint16_t trueChecks;

	public:
	void init();

	void loop();

	private:

	bool tilted();

	void nextCheck();

	bool check();

	int getAngle(int x, int z);

	void signal(bool value);
};

extern _TiltAlarm TiltAlarm;

#endif

