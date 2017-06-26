#pragma once

// SERVO 1 CONFIG
#define SC_SERVO1_PIN 0
#define SC_S1_FULL_LEFT 0
#define SC_S1_ZERO 90
#define SC_S1_FULL_RIGHT 180
// SERVO 2 CONFIG
//#define SC_SERVO2_PIN 1
//#define SC_S2_FULL_LEFT 180
//#define SC_S2_ZERO 90
//#define SC_S2_FULL_RIGHT 0

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
	//static Servo servo2;
	//static uint8_t desiredAngle2;

public:
	/// <summary>
	/// initiates SteeringControl
	/// </summary>
	static void init() {
		servo1.attach(SC_SERVO1_PIN);
		//servo2.attach(SC_SERVO2_PIN);
	}

public:
	/// <summary>
	/// steers car wheels to desired position
	/// </summary>
	/// <param name='percentage'>desired position (-100 - full left, 0 - center, 100 - full right)</param>
	static void steer(int8_t percentage) {
		desiredAngle1 = map(percentage, -100, 100, SC_S1_FULL_LEFT, SC_S1_FULL_RIGHT);
		servo1.write(desiredAngle1);
		//desiredAngle2 = map(percentage, -100, 100, SC_S2_FULL_LEFT, SC_S2_FULL_RIGHT);
		//servo2.write(desiredAngle2);
	}

	// make constructor private
private:
	SteeringControl() {}
};