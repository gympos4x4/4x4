/*
* MotorControl.cpp
*
* Created: 9.11.2017 19:45:11
*  Author: DELTA-PC
*/

#include "MotorControl.h"

_MotorControl MotorControl;

void _MotorControl::init(){
	ctr = 0;
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
}

void _MotorControl::loop(CtrlData& inData, unsigned long millis)
{
	if(millis - lastMillis >= PWM_PERIOD)
	{
		lastMillis = millis;
		if(inData.throttle != 0 && ctr < (abs(inData.throttle) / 20)) //if ctr is within duty cycle:
		{
			if(inData.throttle >= 0) //go forward
			{
				digitalWrite(IN1,HIGH);
				digitalWrite(IN2,LOW);
			}
			else //go backward
			{
				digitalWrite(IN1,LOW);
				digitalWrite(IN2,HIGH);		
			}
		}
		else
		{
			digitalWrite(IN1,LOW);
			digitalWrite(IN2,LOW);
		}
		
		if(ctr < 5)ctr++;
		else ctr = 0;
	}
}

/*void MotorControl::set_throttle(int speed){
if(speed)
{
if(speed > 0)
{
analogWrite(IN1, (speed * 2) + 1);
digitalWrite(IN2, LOW);
}
else
{
digitalWrite(IN1, LOW);
analogWrite(IN2, (speed * -2) + 1);
}
}
else
{
digitalWrite(IN1, LOW);
digitalWrite(IN1, LOW);
}
}*/
