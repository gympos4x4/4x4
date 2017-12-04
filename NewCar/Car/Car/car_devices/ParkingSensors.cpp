// Author: Juraj Marcin

#include "ParkingSensors.h"
#include "../comm.h"

_ParkingSensors ParkingSensors;

void _ParkingSensors::init() {
	pinMode(PS_IR_PIN, OUTPUT);
	eADC::init();
}

int16_t _ParkingSensors::readsensor(int sensor) {
	int16_t high = 0;
	int16_t low = 0;
	if (PS_SENSOR_COUNT > sensor) {
		digitalWrite(PS_IR_PIN, LOW);
		high = eADC::analogRead(sensor);
		digitalWrite(PS_IR_PIN, HIGH);
		low = eADC::analogRead(sensor);
	}
	int16_t res = high - low;
	return res < 0 ? 0 : res;
}

void _ParkingSensors::loop() {
	for (int i = 0; i < PS_SENSOR_COUNT; i++) {
		sensorData[i] = readsensor(i);
	}
}

void _ParkingSensors::update_cardata(CarData& cardata)
{
	for(int i = 0; i < PS_SENSOR_COUNT; i++)
		cardata.parking.sensor_data[i] = sensorData[i];
}

