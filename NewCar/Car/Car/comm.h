#ifndef COMM_H_
#define COMM_H_

#include <stdint.h>

struct CarData
{
	int16_t tilt;
	int8_t battery_percentage;
};

struct CtrlData
{
	int8_t throttle;
	int8_t steer;
};


#endif /* COMM_H_ */
