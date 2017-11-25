#ifndef COMM_H_
#define COMM_H_

#include <stdint.h>

struct CarData
{
	//Misc car data
	int8_t battery_percentage;

	//TiltAlarm
	struct
	{
		int16_t tilt_degrees;
		bool tilted;
	} tilt;

	//Lights
	struct
	{
		int16_t threshold;
		int16_t state;
		bool is_below_threshold;
	} lights;

	//ParkingSensor
	struct {
		int16_t sensor_data[8];
	} parking ;

	int8_t throttleFb;
	uint8_t steerFb;
}; 

struct CtrlData
{
	int8_t throttle;
	//SteeringControl
	uint8_t steering;
}; 


#endif /* COMM_H_ */
