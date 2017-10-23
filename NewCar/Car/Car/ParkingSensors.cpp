// Author: Juraj Marcin

#include "ParkingSensors.h"

void ParkingSensors::init() {
	calibratedMin = 0;
	calibratedMax = 4096;
	calibratedTreshold = 3072;
	beeping = false;
#ifdef PS_BEEDP_PIN
	pinMode(PS_BEEP_PIN, OUTPUT);
#endif // PS_BEEDP_PIN
	eADC::init();
}

void ParkingSensors::calibrateMax(int sensor = 0) {
	calibratedMax = eADC::analogRead(sensor);
}

void ParkingSensors::calibrateMin(int sensor = 0) {
	calibratedMin = eADC::analogRead(sensor);
}

void ParkingSensors::calibrateTreshold(int sensor = 0) {
	calibratedTreshold = eADC::analogRead(sensor);
}

uint16_t ParkingSensors::readsensor(int sensor) {
	if (PS_SENSOR_COUNT > sensor) {
		return map(eADC::analogRead(sensor), 0, 4096, calibratedMin, calibratedMax);
	}
	return 0;
}

void ParkingSensors::loop() {
	uint16_t highest = 4096;
	for (int i = 0; i < PS_SENSOR_COUNT; i++) {
		uint16_t current = readsensor(i);
		if (current > highest) {
			highest = current;
		}
	}
	if (highest > calibratedTreshold) {
		beeping = false;
	} else {
		if (beeping) {
			if (milisstart + PS_BEEP_LENGHT <= millis()) {
				beeping = false;
				milisstart = millis();
			}
		} else {
			if (milisstart + highest <= millis()) {
				beeping = true;
				milisstart = millis();
			}
		}
	}
#ifdef PS_BEEP_PIN
	if (beeping) {
		digitalWrite(PS_BEEP_PIN, PS_BEEP_STATE_HIGH);
	} else {
		digitalWrite(PS_BEEP_PIN, PS_BEEP_STATE_LOW);
	}
#endif // PS_BEEP_PIN
}

