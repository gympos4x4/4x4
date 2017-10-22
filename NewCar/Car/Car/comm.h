

#include <stdint.h>

#ifndef COMM_H_
#define COMM_H_


class CarData
{
	public:
	static const uint8_t len = 2;
	int16_t tilt;
	int8_t battery_percentage;

	/*	void deserialize(byte* stream);
	byte* serialize();*/
};

class CtrlData
{
	public:
	static const uint8_t len = 2;
	int8_t throttle;
	int8_t steer;
	/*void deserialize(byte* stream);
	byte* serialize();*/
};


#endif /* COMM_H_ */
