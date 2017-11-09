#include "comm.h"
/*#define SET8(addr,val) stream[addr] val;
#define SET16(addr,val) stream[addr] = (val & 0xff00) >> 8;stream[addr + 1] = (val & 0xff);

#define GET8(addr) stream[addr]
#define GET16(addr) ((stream[addr] << 8) | stream[addr + 1])


byte* CarData::serialize()
{
	byte[len] stream = new byte[len];
	SET16(0,tilt);
	return stream;
}

void CarData::deserialize(byte* stream)
{
	tilt = stream[0]
}*/