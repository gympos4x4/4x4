
#include <mrf24j.h>
#include <SPI.h>



Mrf24j mrf(34,24,2);
Mrf24j sender(30,31,3);
long last_time;
long tx_interval = 1000;

void setup() {
	Serial.begin(9600);
	
	sender.reset();
	sender.init();
	sender.set_pan(0xcafe);
	sender.address16_write(0x6001);
	sender.set_palna(true);

	delay(50);
	Serial.println(sender.address16_read());

	mrf.reset();
	mrf.init();
	
	mrf.set_pan(0xcafe);
	// This is _our_ address
	mrf.address16_write(0x4202);

	// uncomment if you want to receive any packet on this channel
	//mrf.set_promiscuous(true);
	
	// uncomment if you want to enable PA/LNA external control
	mrf.set_palna(true);
	
	// uncomment if you want to buffer all PHY Payload
	//mrf.set_bufferPHY(true);

	attachInterrupt(0, interrupt_routine, CHANGE); // interrupt 0 equivalent to pin 2(INT0) on ATmega8/168/328
	attachInterrupt(1, interrupt_routine_sender, CHANGE);
	last_time = millis();
	interrupts();
}

void interrupt_routine() {
	mrf.interrupt_handler(); // mrf24 object interrupt routine
}
void interrupt_routine_sender() {
	sender.interrupt_handler(); // mrf24 object interrupt routine
}
void loop() {
	mrf.check_flags(&handle_rx, &handle_tx);
	sender.check_flags(&handle_rx_sender, &handle_tx_sender);
	unsigned long current_time = millis();
	if (current_time - last_time > tx_interval) {
		last_time = current_time;
		Serial.println("Still alive");
		sender.send16(0x4202,"hello");
		//mrf.send16(0x6001, "abcd");
		//Serial.println(mrf.read_short(MRF_PANIDH));*/
		Serial.println(sender.address16_read());
		Serial.println(mrf.address16_read());
	}
}

void handle_rx() {
	Serial.print("received a packet ");Serial.print(mrf.get_rxinfo()->frame_length, DEC);Serial.println(" bytes long");
	
	if(mrf.get_bufferPHY()){
		Serial.println("Packet data (PHY Payload):");
		for (int i = 0; i < mrf.get_rxinfo()->frame_length; i++) {
			Serial.print(mrf.get_rxbuf()[i]);
		}
	}
	
	Serial.println("\r\nASCII data (relevant data):");
	for (int i = 0; i < mrf.rx_datalength(); i++) {
		Serial.write(mrf.get_rxinfo()->rx_data[i]);
	}
	
	Serial.print("\r\nLQI/RSSI=");
	Serial.print(mrf.get_rxinfo()->lqi, DEC);
	Serial.print("/");
	Serial.println(mrf.get_rxinfo()->rssi, DEC);
}

void handle_tx() {
	Serial.println("Receiver just sended a packet. This shouldn't happen!");
	/*if (mrf.get_txinfo()->tx_ok) {
	Serial.println("TX went ok, got ack");
	} else {
	Serial.print("TX failed after ");Serial.print(mrf.get_txinfo()->retries);Serial.println(" retries\n");
	}*/
}
void handle_rx_sender() {
	Serial.println("Sender just received a packet. This shouldn't happen!");
}
void handle_tx_sender() {
	if (mrf.get_txinfo()->tx_ok) {
		Serial.println("TX went ok, got ack");
		} else {
		Serial.print("TX failed after ");Serial.print(mrf.get_txinfo()->retries);Serial.println(" retries\n");
	}
}