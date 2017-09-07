#include "TiltAlarm.h"

void TiltAlarm::init()
{
	init(20);
}

void TiltAlarm::init(byte angle)
{
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

void TiltAlarm::updateAngle(byte angle)
{
	alertAngle = angle;
}

void TiltAlarm::cycle()
{
	nextCheck();
	if (tilted())
	{
		// check tilt with tresholds
		if (millis() >= lastCycle + TA_LED_BLINK_INTERVAL)
		{
			signals(!ledOn);
			ledOn = !ledOn;
		}
	}
	else
	{
		signals(false);
	}
	lastCycle = millis();
}

bool TiltAlarm::tilted()
{
	unsigned char trueChecks = 0;
	for (unsigned char i = 0; i < TA_CHECK_COUNT; i++)
		if (tiltChecks[i])
			trueChecks++; // counts how many checks are true
	if (alreadyTilted) {
		// if car is already tilted then checks if true checks count is over alert disable treshold
		alreadyTilted = trueChecks > TA_CHECK_COUNT * (100 - TA_ALERT_TRESHOLD) / 100;
	}
	else {
		// if car is't tilted then checks if true checks count is over alert enable treshold
		alreadyTilted = trueChecks > TA_CHECK_COUNT * TA_ALERT_TRESHOLD / 100;
	}
	return alreadyTilted;
}

void TiltAlarm::nextCheck()
{
	if (!initDone)
		return; // return if init failed or hasn't been ran
	/*
		GET VALUES FROM SENSOR
	*/
	Wire.beginTransmission(TA_MPU_ADDR);
	Wire.write(0x3B); // start with index 0x3B (x axis [ACCEL_XOUT_H])
	Wire.endTransmission(false);
	Wire.requestFrom(TA_MPU_ADDR, 6, true);
	// request 6 registers <0x3B;0x40> [ACCEL_XOUT_H, ACCEL_XOUT_L, ACCEL_YOUT_H, ACCEL_YOUT_L, ACCEL_ZOUT_H, ACCEL_ZOUT_L]
	acX = Wire.read() << 8 | Wire.read(); // read 0x3B [ACCEL_XOUT_H], 0x3C [ACCEL_XOUT_L]
	Wire.read() << 8 | Wire.read(); // skip 0x3D [ACCEL_YOUT_H], 0x3E [ACCEL_YOUT_L]
	acZ = Wire.read() << 8 | Wire.read(); // read 0x3F [ACCEL_ZOUT_H], 0x40 [ACCEL_ZOUT_L]
	/*
		CHECKS CAR ANGLE
	*/
	if (checkCount < TA_CHECK_COUNT)
	{
		tiltChecks[checkCount] = getAngle(acX, acZ) > TA_ALERT_ANGLE;
		checkCount++;
	}
	else
	{
		checkCount = 0;
	}
}

int TiltAlarm::getAngle(int x, int z)
{
	return abs(degrees(atan2(z, x)));
}

void TiltAlarm::signals(bool value)
{
	if (value)
	{
		digitalWrite(TA_LED_PIN, HIGH);
	}
	else
	{
		digitalWrite(TA_LED_PIN, LOW);
	}
}