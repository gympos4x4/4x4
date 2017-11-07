// Author: Juraj Marcin

#include "Button.h"

Button::Button(uint8_t buttonPin) {
	pin = buttonPin;
	pinMode(buttonPin, INPUT);
	digitalWrite(buttonPin, HIGH);
}

void Button::cycle() {
	buttonDown = false;
	buttonUp = false;
	if (digitalRead(pin)) {
		if (button) {
			buttonUp = true;
		}
		button = false;
	} else {
		if (!button) {
			buttonDown = true;
		}
		button = true;
	}
}

bool Button::GetButton() {
	return button;
}

bool Button::GetButtonDown() {
	return buttonDown;
}

bool Button::GetButtonUp() {
	return buttonUp;
}

