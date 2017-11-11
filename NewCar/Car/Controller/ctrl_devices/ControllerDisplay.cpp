// Author: Juraj Marcin

#include "ControllerDisplay.h";

_ControllerDisplay ControllerDisplay;

uint16_t _ControllerDisplay::bootupConfigure() {
	uint32_t chipid = 0;
	FTImpl->Init(FT_DISPLAY_RESOLUTION, 0); //configure the display to the WQVGA

	delay(20); //for safer side
	chipid = FTImpl->Read32(FT_ROM_CHIPID);
	/* Identify the chip */
	if(FT800_CHIPID != chipid)
	{
		return 1;
	}
	
	FTImpl->DisplayOn();
	FTImpl->AudioOff();
	return 0;
}

void _ControllerDisplay::initDisplay() {
	FTImpl = new FT800IMPL_SPI(FT_CS_PIN, FT_PD_PIN);
	if (bootupConfigure()) {
		displayInitialized = false;
		} else {
		displayInitialized = true;
	}
}

void _ControllerDisplay::startDrawing() {
	if (!displayInitialized)
	return;
	FTImpl->DLStart();
	FTImpl->ClearColorRGB(0, 0, 0);
	FTImpl->Clear(1, 1, 1);
}

void _ControllerDisplay::drawMainGUI() {
	if (!displayInitialized)
	return;
	//SHAFTS
	FTImpl->Begin(FT_LINES);
	FTImpl->ColorRGB(0xFFFFFF);
	FTImpl->LineWidth(10 * 16);
	FTImpl->Vertex2f(80 * 16, 71 * 16);
	FTImpl->Vertex2f(80 * 16, 201 * 16);
	FTImpl->LineWidth(7 * 16);
	FTImpl->Vertex2f(40 * 16, 71 * 16);
	FTImpl->Vertex2f(120 * 16, 71 * 16);
	FTImpl->Vertex2f(40 * 16, 201 * 16);
	FTImpl->Vertex2f(120 * 16, 201 * 16);
	//BATTERY
	FTImpl->Begin(FT_LINE_STRIP);
	FTImpl->LineWidth(3 * 16);
	FTImpl->ColorRGB(0xFFFFFF);
	FTImpl->Vertex2f(167 * 16, 187 * 16);
	FTImpl->Vertex2f(238 * 16, 187 * 16);
	FTImpl->Vertex2f(238 * 16, 223 * 16);
	FTImpl->Vertex2f(167 * 16, 223 * 16);
	FTImpl->Vertex2f(167 * 16, 212 * 16);
	FTImpl->Vertex2f(162 * 16, 212 * 16);
	FTImpl->Vertex2f(162 * 16, 198 * 16);
	FTImpl->Vertex2f(167 * 16, 198 * 16);
	FTImpl->Vertex2f(167 * 16, 187 * 16);
	//SPEEDOMETER
	FTImpl->ColorRGB(0xFFFFFF);
	FTImpl->Cmd_BGColor(SPD_BG_COLOR);
	FTImpl->Cmd_Gauge(358, 136, 110, FT_OPT_NOPOINTER, SPD_MAJOR, SPD_MINOR, 0, SPD_RANGE);
	//LIGHTS
	FTImpl->ColorRGB(LII_DAY_COLOR);
	FTImpl->LineWidth(2 * 16);
	FTImpl->Begin(FT_LINE_STRIP);
	FTImpl->Vertex2f(219 * 16, 146 * 16);
	FTImpl->Vertex2f(228 * 16, 146 * 16);
	FTImpl->Vertex2f(234 * 16, 151 * 16);
	FTImpl->Vertex2f(234 * 16, 167 * 16);
	FTImpl->Vertex2f(228 * 16, 172 * 16);
	FTImpl->Vertex2f(219 * 16, 172 * 16);
	FTImpl->Vertex2f(219 * 16, 146 * 16);
	FTImpl->Begin(FT_LINES);
	FTImpl->Vertex2f(215 * 16, 151 * 16);
	FTImpl->Vertex2f(204 * 16, 155 * 16);
	FTImpl->Vertex2f(215 * 16, 159 * 16);
	FTImpl->Vertex2f(204 * 16, 163 * 16);
	FTImpl->Vertex2f(215 * 16, 167 * 16);
	FTImpl->Vertex2f(204 * 16, 171 * 16);
}

void _ControllerDisplay::drawWheels(int8_t steer) {
	if (!displayInitialized)
	return;
	int8_t b = map(steer, WHL_MIN, WHL_MAX, -8, 8);
	int8_t c = 16;
	int8_t a = sqrt(pow(c, 2) - pow(b, 2));
	FTImpl->Begin(FT_LINES);
	FTImpl->LineWidth(8 * 16);
	FTImpl->ColorRGB(WHL_COLOR);
	//FRONT WHEELS
	FTImpl->Vertex2f((32 + b) * 16, (71 - a) * 16);
	FTImpl->Vertex2f((32 - b) * 16, (71 + a) * 16);
	FTImpl->Vertex2f((128 + b) * 16, (71 - a) * 16);
	FTImpl->Vertex2f((128 - b) * 16, (71 + a) * 16);
	//REAR WHEELS
	FTImpl->Vertex2f((32 - b) * 16, (201 - a) * 16);
	FTImpl->Vertex2f((32 + b) * 16, (201 + a) * 16);
	FTImpl->Vertex2f((128 - b) * 16, (201 - a) * 16);
	FTImpl->Vertex2f((128 + b) * 16, (201 + a) * 16);

}

void _ControllerDisplay::drawSpeedometer(int8_t speed) {
	if (!displayInitialized)
	return;
	#ifdef SPD_DRAW_TEXT
	FTImpl->ColorRGB(0xFFFFFF);
	char buf[3];
	sprintf(buf, "%03i", abs(speed));
	FTImpl->Cmd_Text(358, 180, 29, FT_OPT_CENTER, buf);
	FTImpl->Cmd_Text(358, 190, 26, FT_OPT_CENTERX, "km/h");
	#endif
	FTImpl->ColorRGB(SPD_COLOR);
	FTImpl->Cmd_Gauge(358, 136, 110, FT_OPT_NOBACK | FT_OPT_NOTICKS, SPD_MAJOR, SPD_MINOR, abs(speed), SPD_RANGE);
	FTImpl->ColorRGB(0xFFFFFF);
	if (speed > 0) {
		FTImpl->Cmd_Text(358, 220, 31, FT_OPT_CENTER, "D");
		} else if (speed < 0) {
		FTImpl->Cmd_Text(358, 220, 31, FT_OPT_CENTER, "R");
		} else {
		FTImpl->Cmd_Text(358, 220, 31, FT_OPT_CENTER, "P");
	}
}

void _ControllerDisplay::drawCarBattery(int8_t percentage) {
	if (!displayInitialized)
	return;
	if (percentage < 10) {
		FTImpl->ColorRGB(BAT_WAR_COL);
		} else if (percentage < 25) {
		FTImpl->ColorRGB(BAT_LOW_COL);
		} else {
		FTImpl->ColorRGB(0xFFFFFF);
	}
	char buf[3];
	sprintf(buf, "%3i", percentage);
	FTImpl->Cmd_Text(233, 205, 30, FT_OPT_RIGHTX | FT_OPT_CENTERY, buf);
}

void _ControllerDisplay::drawRotation(int8_t z) {
	if (!displayInitialized)
	return;
	if (abs(z) > 45) {
		FTImpl->ColorRGB(ROT_WAR_COL);
		} else if (abs(z) > 20) {
		FTImpl->ColorRGB(ROT_LOW_COL);
		} else {
		FTImpl->ColorRGB(0xFFFFFF);
	}
	FTImpl->Begin(FT_POINTS);
	FTImpl->PointSize(40 * 16);
	FTImpl->Vertex2f(200 * 16, 100 * 16);
	FTImpl->ColorRGB(ROT_COLOR);
	FTImpl->PointSize(5 * 16);
	FTImpl->Vertex2f(200 * 16, 100 * 16);
	int8_t a = 30 * cos(radians(z));
	int8_t b = 30 * sin(radians(z));
	FTImpl->LineWidth(3 * 16);
	FTImpl->Begin(FT_LINES);
	FTImpl->Vertex2f((200 + a) * 16, (100 + b) * 16);
	FTImpl->Vertex2f((200 - a) * 16, (100 - b) * 16);
}

void _ControllerDisplay::drawLights(bool headlights) {
	if (!displayInitialized)
	return;
	if (headlights) {
		FTImpl->LineWidth(2 * 16);
		FTImpl->ColorRGB(LII_HEAD_COLOR);
		FTImpl->Begin(FT_LINE_STRIP);
		FTImpl->Vertex2f(179 * 16, 146 * 16);
		FTImpl->Vertex2f(188 * 16, 146 * 16);
		FTImpl->Vertex2f(194 * 16, 151 * 16);
		FTImpl->Vertex2f(194 * 16, 167 * 16);
		FTImpl->Vertex2f(188 * 16, 172 * 16);
		FTImpl->Vertex2f(179 * 16, 172 * 16);
		FTImpl->Vertex2f(179 * 16, 146 * 16);
		FTImpl->Begin(FT_LINES);
		FTImpl->Vertex2f(175 * 16, 151 * 16);
		FTImpl->Vertex2f(164 * 16, 151 * 16);
		FTImpl->Vertex2f(175 * 16, 159 * 16);
		FTImpl->Vertex2f(164 * 16, 159 * 16);
		FTImpl->Vertex2f(175 * 16, 167 * 16);
		FTImpl->Vertex2f(164 * 16, 167 * 16);
	}
}

void _ControllerDisplay::drawFrontParkingSensors(int16_t sensor0, int16_t sensor1, int16_t sensor2, int16_t sensor3) {
	if (!displayInitialized)
	return;
	sensor0 = map(sensor0, 0, 4096, PRS_FRONT_MIN, PRS_FRONT_MAX);
	sensor1 = map(sensor1, 0, 4096, PRS_FRONT_MIN, PRS_FRONT_MAX);
	sensor2 = map(sensor2, 0, 4096, PRS_FRONT_MIN, PRS_FRONT_MAX);
	sensor3 = map(sensor3, 0, 4096, PRS_FRONT_MIN, PRS_FRONT_MAX);
	FTImpl->Begin(FT_EDGE_STRIP_A);
	FTImpl->ColorRGB(PRS_COLOR);
	FTImpl->Vertex2f(24 * 16, sensor0 * 16);
	FTImpl->Vertex2f(52 * 16, sensor0 * 16);
	FTImpl->Vertex2f(52 * 16, sensor1 * 16);
	FTImpl->Vertex2f(80 * 16, sensor1 * 16);
	FTImpl->Vertex2f(80 * 16, sensor2 * 16);
	FTImpl->Vertex2f(108 * 16, sensor2 * 16);
	FTImpl->Vertex2f(108 * 16, sensor3 * 16);
	FTImpl->Vertex2f(136 * 16, sensor3 * 16);
}

void _ControllerDisplay::drawRearParkingSensors(int16_t sensor4, int16_t sensor5, int16_t sensor6, int16_t sensor7) {
	if (!displayInitialized)
	return;
	sensor4 = map(sensor4, 0, 4096, PRS_REAR_MIN, PRS_REAR_MAX);
	sensor5 = map(sensor5, 0, 4096, PRS_REAR_MIN, PRS_REAR_MAX);
	sensor6 = map(sensor6, 0, 4096, PRS_REAR_MIN, PRS_REAR_MAX);
	sensor7 = map(sensor7, 0, 4096, PRS_REAR_MIN, PRS_REAR_MAX);
	FTImpl->Begin(FT_EDGE_STRIP_B);
	FTImpl->ColorRGB(PRS_COLOR);
	FTImpl->Vertex2f(24 * 16, sensor4 * 16);
	FTImpl->Vertex2f(52 * 16, sensor4 * 16);
	FTImpl->Vertex2f(52 * 16, sensor5 * 16);
	FTImpl->Vertex2f(80 * 16, sensor5 * 16);
	FTImpl->Vertex2f(80 * 16, sensor6 * 16);
	FTImpl->Vertex2f(108 * 16, sensor6 * 16);
	FTImpl->Vertex2f(108 * 16, sensor7 * 16);
	FTImpl->Vertex2f(136 * 16, sensor7 * 16);
}

void _ControllerDisplay::drawCalibrationNotice(int8_t joystick, int8_t direction) {
	String notice = "Move ";
	if (joystick == 0) {
		notice += "left";
		} else {
		notice += "right";
	}
	notice += " joystick ";
	switch (direction) {
		case 0:
		notice += "upwards";
		break;
		case 1:
		notice += "downwards";
		break;
		case 2:
		notice += "to the right";
		break;
		case 3:
		notice += "to the left";
		break;
	}
	FTImpl->ColorRGB(0xFFFFFF);
	FTImpl->Cmd_Text(240, 125, 30, FT_OPT_CENTER, notice.c_str());
}

void _ControllerDisplay::endDrawing() {
	if (!displayInitialized)
	return;
	FTImpl->DLEnd();
	FTImpl->Finish();
}