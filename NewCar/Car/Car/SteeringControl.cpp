// Author: Juraj Marcin

#include "SteeringControl.h"
#include "../comm.h"

_SteeringControl SteeringControl;

void _SteeringControl::init() {
	servo1.attach(SC_SERVO1_PIN);
#ifdef SC_SERVO2_PIN
	servo2.attach(SC_SERVO2_PIN);
#endif // SC_SERVO2_PIN
}

void _SteeringControl::steer(int8_t percentage) {
	desiredAngle1 = map(percentage, -100, 100, SC_S1_FULL_LEFT, SC_S1_FULL_RIGHT);
	servo1.write(desiredAngle1);
#ifdef SC_SERVO2_PIN
	desiredAngle2 = map(percentage, -100, 100, SC_S2_FULL_LEFT, SC_S2_FULL_RIGHT);
	servo2.write(desiredAngle2);
#endif // SC_SERVO2_PIN
}

void _SteeringControl::update_cardata(CarData& cardata)
{
	cardata.steering.servo_angle = SteeringControl.servo1.read();
}
