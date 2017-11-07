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
		int16_t sensor0data;
		int16_t sensor1data;
		int16_t sensor2data;
		int16_t sensor3data;
		int16_t sensor4data;
		int16_t sensor5data;
		int16_t sensor6data;
		int16_t sensor7data;
	} parking ;

	//SteeringControl
	struct{
		int16_t servo_angle;
		int16_t servo2_angle;
	} steering;
}; //29 bytes;

struct CtrlData
{
	int8_t throttle;
	int8_t steer;
}; //2 bytes


#endif /* COMM_H_ */
