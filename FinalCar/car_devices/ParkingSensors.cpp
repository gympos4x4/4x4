// Author: Juraj Marcin

#include "ParkingSensors.h"
#include "../comm.h"

_ParkingSensors ParkingSensors;

void _ParkingSensors::init() {
	calibratedMin = 0;
	calibratedMax = 4096;
	calibratedThreshold = 3072;
	beeping = false;
#ifdef PS_BEEP_PIN
	pinMode(PS_BEEP_PIN, OUTPUT);
#endif // PS_BEEP_PIN
	pinMode(PS_IR_PIN, OUTPUT);
	eADC::init();
}

void _ParkingSensors::calibrateMax(int sensor = 0) {
	digitalWrite(PS_IR_PIN, HIGH);
	eADC::analogRead(sensor);
	digitalWrite(PS_IR_PIN, LOW);
}

void _ParkingSensors::calibrateMin(int sensor = 0) {
	digitalWrite(PS_IR_PIN, HIGH);
	calibratedMin = eADC::analogRead(sensor);
	digitalWrite(PS_IR_PIN, LOW);
}

void _ParkingSensors::calibrateThreshold(int sensor = 0) {
	digitalWrite(PS_IR_PIN, HIGH);
	calibratedThreshold = eADC::analogRead(sensor);
	digitalWrite(PS_IR_PIN, LOW);
}

int16_t _ParkingSensors::readsensor(int sensor) {
	int16_t high = 0;
	int16_t low = 0;
	if (PS_SENSOR_COUNT > sensor) {
		digitalWrite(PS_IR_PIN, HIGH);
		high = map(eADC::analogRead(sensor), 0, 4096, calibratedMin, calibratedMax);
		digitalWrite(PS_IR_PIN, LOW);
		low = map(eADC::analogRead(sensor), 0, 4096, calibratedMin, calibratedMax);
	}
	return high - low;
}

void _ParkingSensors::loop() {
	int16_t highest = 4096;
	for (int i = 0; i < PS_SENSOR_COUNT; i++) {
		int16_t current = readsensor(i);
		sensorData[i] = current;
		if (current > highest) {
			highest = current;
		}
	}
#ifdef PS_BEEP_PIN
	if (highest > calibratedThreshold) {
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
	if (beeping) {
		digitalWrite(PS_BEEP_PIN, PS_BEEP_STATE_HIGH);
	} else {
		digitalWrite(PS_BEEP_PIN, PS_BEEP_STATE_LOW);
	}
#endif // PS_BEEP_PIN
}

void _ParkingSensors::update_cardata(CarData& cardata)
{
	for(int i = 0; i < 8; i++)
		cardata.parking.sensor_data[i] = sensorData[i];
}

