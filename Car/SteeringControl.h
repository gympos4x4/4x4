// SteeringControl.h
// Author: Juraj Marcin

#ifndef _STEERINGCONTROL_h
#define _STEERINGCONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// SERVO 1 CONFIG
#define SC_SERVO1_PIN 0
// TODO: update angles, so it matches chassi
#define SC_S1_FULL_LEFT 0
#define SC_S1_ZERO 90
#define SC_S1_FULL_RIGHT 180
// SERVO 2 CONFIG
//#define SC_SERVO2_PIN 1
#define SC_S2_FULL_LEFT 180
#define SC_S2_ZERO 90
#define SC_S2_FULL_RIGHT 0

#include <Servo.h>

/// <summary>
/// servo steering control
/// </summary>
/// <example>
/// Example implementation
/// <code>
/// #include "SteeringControl.h"
/// void setup() {
/// 	SteeringControl::init();
/// }
/// void loop() {
/// 	SteeringControl::steer(0);
/// 	delay(1000);
/// 	SteeringControl::steer(-100);
/// 	delay(1000);
/// 	SteeringControl::steer(0);
/// 	delay(1000);
/// 	SteeringControl::steer(100);
/// 	delay(1000);
/// }
/// </code>
/// </example>

class SteeringControl {

	static Servo servo1;
	static uint8_t desiredAngle1;
#ifdef SC_SERVO2
	static Servo servo2;
	static uint8_t desiredAngle2;
#endif // SC_SERVO2

public:
	/// <summary>
	/// initiates SteeringControl
	/// </summary>
	static void init();

public:
	/// <summary>
	/// steers car wheels to desired position
	/// </summary>
	/// <param name='percentage'>desired position (-100 - full left, 0 - center, 100 - full right)</param>
	static void steer(int8_t percentage);

	// make constructor private
private:
	SteeringControl();
};

#endif

