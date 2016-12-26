// ISerialise.h

#ifndef _ISERIALISE_h
#define _ISERIALISE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <assert.h>

//Interface indicating that this class can be serialised and deserialised
class ISerializable
{
public:
	/* determinate the size	of buffer needed to store all data of this instance */
	virtual int get_size() const = 0;

	/* Copy all synced variables from class to buffer. Provides basic error checking.
	*
	* @param[in]	buffer	Pointer to buffer where synced information is stored
	*/
	virtual void serialize(char* buffer) const = 0;

	/* Copy all synced variables from buffer to class. Provides basic error checking.
	*
	* @param[in]	buffer	Pointer to buffer where synced information is stored
	*/
	virtual void deserialize(char* buffer) = 0;
};

#endif

