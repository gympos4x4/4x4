// Autor:Peter Ridilla

#ifndef _MOTORCONTROL_h
#define _MOTORCONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "../comm.h"

#define IN1 A3
#define IN2 A2
//period of single 1/255 of the whole PWM cycle
#define PWM_PERIOD 10

class _MotorControl{
	
	
	public:
	
	_MotorControl(){}
	~_MotorControl(){}
	
	void init();
	
	//static void set_throttle(int speed);
	
	void loop(CtrlData& inData);
};

extern _MotorControl MotorControl;
#endif

