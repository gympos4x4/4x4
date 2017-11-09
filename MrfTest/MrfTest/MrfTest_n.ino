
#include "mrf24j_c.h"
#include <SPI.h>
#include "comm.h"


Mrf24j mrf(34,24,2);
long last_time;
long tx_interval = 1000;
CarData cardata;
void setup() {
	Serial.begin(9600);

	mrf.reset();
	mrf.init();
	mrf.set_promiscuous(true);
	mrf.set_pan(0xcafe);
	mrf.address16_write(0x4202);
	mrf.set_palna(true);
	attachInterrupt(0, interrupt_routine, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
	last_time = millis();
	interrupts();
}

void interrupt_routine() {
	mrf.interrupt_handler(); // mrf24 object interrupt routine
}
void loop() {
	mrf.check_flags(&handle_rx, &handle_tx);
	unsigned long current_time = millis();
	if (current_time - last_time > tx_interval) {
		last_time = current_time;
		Serial.println("Still alive");

		Serial.println(mrf.address16_read());
	}
}

void handle_rx() {
	Serial.print("Received data: ");
	mrf.recv_car_data(&cardata);
	Serial.println(cardata.tilt);
}

void handle_tx() {
	Serial.println("Receiver just sent a packet. This shouldn't happen!");

}
