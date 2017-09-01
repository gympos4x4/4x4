/*
 Name:		SharedLib.h
 Created:	1/4/17 2:40:51 AM
 Author:	Matej
 Editor:	http://www.visualmicro.com
*/

#ifndef _SharedLib_h
#define _SharedLib_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "libs/mrf24j.h"
#include "ISerializable.h"
#include "SyncManager.h"

#endif

