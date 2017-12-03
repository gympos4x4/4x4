/*
* V3Test.ino
*
* Created: 11/28/2017 6:49:08 PM
* Author: DELTA-PC
*/

void setup()
{
	pinMode(3, OUTPUT);
	pinMode(A3, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(7, OUTPUT);
	//digitalWrite(3,HIGH);
	/* add setup code here, setup code runs once when the processor starts */

}

void loop()
{
	analogWrite(3,127);
	digitalWrite(7,HIGH);
	digitalWrite(A3, HIGH);
	digitalWrite(A2, LOW);
	delay(1000);
	analogWrite(3,255);
	digitalWrite(7,LOW);
	digitalWrite(A3, LOW);
	digitalWrite(A2, LOW);
	delay(1000);
	digitalWrite(7,HIGH);
	digitalWrite(A3, LOW);
	digitalWrite(A2, HIGH);
	delay(1000);
	/* add main program code here, this code starts again each time it ends */

}
