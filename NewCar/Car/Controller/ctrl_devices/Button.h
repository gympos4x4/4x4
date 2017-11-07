// Author: Juraj Marcin

#ifndef _BUTTON_h
#define _BUTTON_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Button {

	private:

	int pin;
	bool buttonDown;
	bool buttonUp;
	bool button;

	public:

	/// <summary>
	/// create new button
	/// </summary>
	/// <param name='buttonPin'>pin button is attached to</param>
	Button(uint8_t buttonPin);
	~Button() {}

	/// <summary>
	/// executes one check cycle
	/// </summary>
	void cycle();

	/// <summary>
	/// returns true if the button is pressed down
	/// </summary>
	bool GetButton();

	/// <summary>
	/// returns true if the button has been just pressed down
	/// </summary>
	bool GetButtonDown();

	/// <summary>
	/// returns true if the button has been just released
	/// </summary>
	bool GetButtonUp();
};

#endif