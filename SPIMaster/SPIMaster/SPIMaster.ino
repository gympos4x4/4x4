/*
 * SPIMaster.ino
 *
 * Created: 11/18/17 9:43:12 PM
 * Author: Matej
 */
 #include <SPI.h>
void setup()
{
	Serial.begin(9600);
	  /* add setup code here, setup code runs once when the processor starts */
	  SPI.begin();
}

void loop()
{
	Serial.println("Txxing request");
	SPI.transfer(0xcd);
	  /* add main program code here, this code starts again each time it ends */
	byte response = SPI.transfer(0);
	Serial.print("Response: ");
	Serial.println((int)response);
	delay(500);
}
