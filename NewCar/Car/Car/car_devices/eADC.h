// eADC.h
// Author: Juraj Marcin

#ifndef _EADC_h
#define _EADC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define ADC_CSPIN 10

#include <SPI.h>

/// <summary>
/// communication with external ADC (MCP3208)
/// </summary>
/// <example>
/// Example implementation
/// <code>
/// #include "eADC.h"
/// void setup() {
///		eADC::init();
/// }
/// void loop() {
/// 	uint16_t result = eADC::analogRead(0);
/// }
/// </code>
/// </example>

class eADC {

public:
	/// <summary>
	/// initiates eADC
	/// </summary>
	static void init();

public:
	/// <summary>
	/// reads ADC value on selected channel
	/// </summary>
	/// <param name='channel'>ADC channel (0-7)</param>
	static uint16_t analogRead(int channel);

	// make constructor private
private:
	eADC();
};

#endif

