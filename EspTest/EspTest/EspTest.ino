#include "SPI.h"

/*
 * EspTest.ino
 *
 * Created: 11/18/17 4:51:19 PM
 * Author: Matej
 */ 

 char buf[64];

void setup()
{
	  pinMode(53, OUTPUT);
	  digitalWrite(53, HIGH);

	  delay(1000);
}

void loop()
{
	digitalWrite(53, LOW);
	uint32_t status = (SPI.transfer(0) | ((uint32_t)(SPI.transfer(0)) << 8) | ((uint32_t)(SPI.transfer(0)) << 16) | ((uint32_t)(SPI.transfer(0)) << 24));
	digitalWrite(53, HIGH);
	
	vsprintf(buf, "%#032x", status);
	Serial.println(status);
	
	delay(1000);
}
