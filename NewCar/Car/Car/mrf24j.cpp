/**
* mrf24j.cpp, Karl Palsson, 2011, karlp@tweak.net.au
* modified bsd license / apache license
*/

//;_; y u no work

#include "mrf24j.h"

// aMaxPHYPacketSize = 127, from the 802.15.4-2006 standard.
static uint8_t rx_buf[127];

// essential for obtaining the data frame only
// bytes_MHR = 2 Frame control + 1 sequence number + 2 panid + 2 shortAddr Destination + 2 shortAddr Source
static int bytes_MHR = 9;
static int bytes_FCS = 2; // FCS length = 2
static int bytes_nodata = bytes_MHR + bytes_FCS; // no_data bytes in PHY payload,  header length + FCS

static int ignoreBytes = 0; // bytes to ignore, some modules behaviour.

static boolean bufPHY = false; // flag to buffer all bytes in PHY Payload, or not

volatile uint8_t flag_got_rx;
volatile uint8_t flag_got_tx;

static rx_info_t rx_info;
static tx_info_t tx_info;


/**
* Constructor MRF24J Object.
* @param PIN_RESET, @param pin_chip_select, @param PIN_INTERRUPT
*/
Mrf24j::Mrf24j(int PIN_RESET, int pin_chip_select, int PIN_INTERRUPT) {
	_pin_reset = PIN_RESET;
	_pin_cs = pin_chip_select;
	_pin_int = PIN_INTERRUPT;

	pinMode(_pin_reset, OUTPUT);
	pinMode(_pin_cs, OUTPUT);
	pinMode(_pin_int, INPUT);

	SPI.setBitOrder(MSBFIRST) ;
	SPI.setDataMode(SPI_MODE0);
	//	SPI.setClockDivider(255);
	SPI.begin();
}

void Mrf24j::start_tx(word dest16, byte len)
{
	uint8_t i = 0;
	write_long(i++, bytes_MHR); // header length
	// +ignoreBytes is because some module seems to ignore 2 bytes after the header?!.
	// default: ignoreBytes = 0;
	write_long(i++, bytes_MHR+ignoreBytes+len);

	// 0 | pan compression | ack | no security | no data pending | data frame[3 bits]
	write_long(i++, 0b01100001); // first byte of Frame Control
	// 16 bit source, 802.15.4 (2003), 16 bit dest,
	write_long(i++, 0b10001000); // second byte of frame control
	write_long(i++, 1);  // sequence number 1

	word pan = get_pan();

	write_long(i++, pan & 0xff);  // dest panid
	write_long(i++, pan >> 8);
	write_long(i++, dest16 & 0xff);  // dest16 low
	write_long(i++, dest16 >> 8); // dest16 high

	word src16 = address16_read();
	write_long(i++, src16 & 0xff); // src16 low
	write_long(i++, src16 >> 8); // src16 high
	
	write_long(11, 42); //Always write first byte as '1'. This servers as data reception and integrity check.

}
void Mrf24j::finish_tx(void)
{
	// ack on, and go!
	write_short(MRF_TXNCON, (1<<MRF_TXNACKREQ | 1<<MRF_TXNTRIG));
}
/*==================================================================
======================DATA SYNC======================
==================================================================
*/
//R/W Operators:
#define WRITE8(val) do { write_long(i++, val); } while(0)
#define READ8(val)  do { val = rx_info.rx_data[i]; i += 1; } while(0)

#ifdef CAR
void Mrf24j::send_car_data(CarData* data)
{
	byte i = 12;

	for(byte iter = 0; iter < sizeof(CarData); iter++)
	{
		WRITE8( ((byte*)data)[iter] );
	}
}
void Mrf24j::recv_ctrl_data(CtrlData* data)
{
	byte i = 0;

	for(byte iter = 0; iter < sizeof(CtrlData); iter++)
	{
		READ8( ((byte*)data)[iter] );
	}
}
#else
void Mrf24j::recv_car_data(CarData* data)
{
	byte i = 0;

	for(byte iter = 0; iter < sizeof(CarData); iter++)
	{
		READ8( ((byte*)data)[iter] );
	}
}
void Mrf24j::send_ctrl_data(CtrlData* data)
{
	byte i = 12;

	for(byte iter = 0; iter < sizeof(CtrlData); iter++)
	{
		WRITE8( ((byte*)data)[iter] );
	}
}
#endif

void Mrf24j::reset(void) {
	digitalWrite(_pin_reset, LOW);
	delay(10);  // just my gut
	digitalWrite(_pin_reset, HIGH);
	delay(20);  // from manual
}

byte Mrf24j::read_short(byte address) {
	digitalWrite(_pin_cs, LOW);
	// 0 top for short addressing, 0 bottom for read
	SPI.transfer(address<<1 & 0b01111110);
	byte ret = SPI.transfer(0x00);
	digitalWrite(_pin_cs, HIGH);
	return ret;
}

byte Mrf24j::read_long(word address) {
	digitalWrite(_pin_cs, LOW);
	byte ahigh = address >> 3;
	byte alow = address << 5;
	SPI.transfer(0x80 | ahigh);  // high bit for long
	SPI.transfer(alow);
	byte ret = SPI.transfer(0);
	digitalWrite(_pin_cs, HIGH);
	return ret;
}


void Mrf24j::write_short(byte address, byte data) {
	digitalWrite(_pin_cs, LOW);
	// 0 for top short address, 1 bottom for write
	SPI.transfer((address<<1 & 0b01111110) | 0x01);
	SPI.transfer(data);
	digitalWrite(_pin_cs, HIGH);
}

void Mrf24j::write_long(word address, byte data) {
	digitalWrite(_pin_cs, LOW);
	byte ahigh = address >> 3;
	byte alow = address << 5;
	SPI.transfer(0x80 | ahigh);  // high bit for long
	SPI.transfer(alow | 0x10);  // last bit for write
	SPI.transfer(data);
	digitalWrite(_pin_cs, HIGH);
}

word Mrf24j::get_pan(void) {
	byte panh = read_short(MRF_PANIDH);
	return panh << 8 | read_short(MRF_PANIDL);
}

void Mrf24j::set_pan(word panid) {
	write_short(MRF_PANIDH, panid >> 8);
	write_short(MRF_PANIDL, panid & 0xff);
}

void Mrf24j::address16_write(word address16) {
	write_short(MRF_SADRH, address16 >> 8);
	write_short(MRF_SADRL, address16 & 0xff);
}

word Mrf24j::address16_read(void) {
	byte a16h = read_short(MRF_SADRH);
	return a16h << 8 | read_short(MRF_SADRL);
}



void Mrf24j::set_interrupts(void) {
	// interrupts for rx and tx normal complete
	//write_short(MRF_INTCON, 0b11111110);
}

/** use the 802.15.4 channel numbers..
*/
void Mrf24j::set_channel(byte channel) {
	write_long(MRF_RFCON0, (((channel - 11) << 4) | 0x03));
}

void Mrf24j::init(void) {
	/*
	// Seems a bit ridiculous when I use reset pin anyway
	write_short(MRF_SOFTRST, 0x7); // from manual
	while (read_short(MRF_SOFTRST) & 0x7 != 0) {
	; // wait for soft reset to finish
	}
	*/
	write_short(MRF_PACON2, 0x98); // – Initialize FIFOEN = 1 and TXONTS = 0x6.
	write_short(MRF_TXSTBL, 0x95); // – Initialize RFSTBL = 0x9.

	write_long(MRF_RFCON0, 0x03); // – Initialize RFOPT = 0x03.
	write_long(MRF_RFCON1, 0x01); // – Initialize VCOOPT = 0x02.
	write_long(MRF_RFCON2, 0x80); // – Enable PLL (PLLEN = 1).
	
	write_long(MRF_RFCON3,0x28); //Set tx power to -3.7 (see mrf24j40mc datasheet)
	
	write_long(MRF_RFCON6, 0x90); // – Initialize TXFIL = 1 and 20MRECVR = 1.
	write_long(MRF_RFCON7, 0x80); // – Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator).
	write_long(MRF_RFCON8, 0x10); // – Initialize RFVCO = 1.
	write_long(MRF_SLPCON1, 0x21); // – Initialize CLKOUTEN = 1 and SLPCLKDIV = 0x01.

	//  Configuration for nonbeacon-enabled devices (see Section 3.8 “Beacon-Enabled and
	//  Nonbeacon-Enabled Networks”):
	write_short(MRF_BBREG2, 0x80); // Set CCA mode to ED
	write_short(MRF_CCAEDTH, 0x60); // – Set CCA ED threshold.
	write_short(MRF_BBREG6, 0x40); // – Set appended RSSI value to RXFIFO.
	set_interrupts();
	set_channel(12);
	// max power is by default.. just leave it...
	// Set transmitter power - See “REGISTER 2-62: RF CONTROL 3 REGISTER (ADDRESS: 0x203)”.
	write_short(MRF_RFCTL, 0x04); //  – Reset RF state machine.
	write_short(MRF_RFCTL, 0x00); // part 2
	delay(1); // delay at least 192usec
}

/*
Read data in FIFO without interrupt
*/
bool Mrf24j::read_rxdata()
{
	rx_disable();
	// read start of rxfifo for, has 2 bytes more added by FCS. frame_length = m + n + 2
	uint8_t frame_length = read_long(0x300);

	// buffer all bytes in PHY Payload
	if(bufPHY){
		int rb_ptr = 0;
		for (int i = 0; i < frame_length; i++) { // from 0x301 to (0x301 + frame_length -1)
			rx_buf[rb_ptr++] = read_long(0x301 + i);
		}
	}

	// buffer data bytes
	int rd_ptr = 0;
	// from (0x301 + bytes_MHR) to (0x301 + frame_length - bytes_nodata - 1)
	
	//Read first byte. This should always be '1', else we know that either incorrect or no data has been received.
	byte receivedData = read_long(0x301 + bytes_MHR);
	//if(receivedData != 1) return false;
	
	//Continue reading next bytes
	for (int i = 1; i < rx_datalength(); i++) {
		rx_info.rx_data[rd_ptr++] = read_long(0x301 + bytes_MHR + i);
	}

	rx_info.frame_length = frame_length;
	// same as datasheet 0x301 + (m + n + 2) <-- frame_length
	rx_info.lqi = read_long(0x301 + frame_length);
	// same as datasheet 0x301 + (m + n + 3) <-- frame_length + 1
	rx_info.rssi = read_long(0x301 + frame_length + 1);

	rx_enable();
	return receivedData;
}

/**
* Call this from within an interrupt handler connected to the MRFs output
* interrupt pin.  It handles reading in any data from the module, and letting it
* continue working.
* Only the most recent data is ever kept.
*/
void Mrf24j::interrupt_handler(void) {
	uint8_t last_interrupt = read_short(MRF_INTSTAT);
	if (last_interrupt & MRF_I_RXIF) {
	flag_got_rx++;
	// read out the packet data...
	noInterrupts();
	rx_disable();
	// read start of rxfifo for, has 2 bytes more added by FCS. frame_length = m + n + 2
	uint8_t frame_length = read_long(0x300);

	// buffer all bytes in PHY Payload
	if(bufPHY){
	int rb_ptr = 0;
	for (int i = 0; i < frame_length; i++) { // from 0x301 to (0x301 + frame_length -1)
	rx_buf[rb_ptr++] = read_long(0x301 + i);
	}
	}

	// buffer data bytes
	int rd_ptr = 0;
	// from (0x301 + bytes_MHR) to (0x301 + frame_length - bytes_nodata - 1)
	for (int i = 0; i < rx_datalength(); i++) {
	rx_info.rx_data[rd_ptr++] = read_long(0x301 + bytes_MHR + i);
	}

	rx_info.frame_length = frame_length;
	// same as datasheet 0x301 + (m + n + 2) <-- frame_length
	rx_info.lqi = read_long(0x301 + frame_length);
	// same as datasheet 0x301 + (m + n + 3) <-- frame_length + 1
	rx_info.rssi = read_long(0x301 + frame_length + 1);

	rx_enable();
	interrupts();
	}
	if (last_interrupt & MRF_I_TXNIF) {
	flag_got_tx++;
	uint8_t tmp = read_short(MRF_TXSTAT);
	// 1 means it failed, we want 1 to mean it worked.
	tx_info.tx_ok = !(tmp & ~(1 << TXNSTAT));
	tx_info.retries = tmp >> 6;
	tx_info.channel_busy = (tmp & (1 << CCAFAIL));
	}
	write_short(MRF_INTSTAT,0);
}


/**
* Call this function periodically, it will invoke your nominated handlers
*/
void Mrf24j::check_flags(void (*rx_handler)(void), void (*tx_handler)(void)){
	// TODO - we could check whether the flags are > 1 here, indicating data was lost?
	if (flag_got_rx) {
		flag_got_rx = 0;
		rx_handler();
	}
	if (flag_got_tx) {
		flag_got_tx = 0;
		tx_handler();
	}
}


/**
* Set RX mode to promiscuous, or normal
*/
void Mrf24j::set_promiscuous(boolean enabled) {
	if (enabled) {
		write_short(MRF_RXMCR, 0x01);
		} else {
		write_short(MRF_RXMCR, 0x00);
	}
}

rx_info_t * Mrf24j::get_rxinfo(void) {
	return &rx_info;
}

tx_info_t * Mrf24j::get_txinfo(void) {
	return &tx_info;
}

uint8_t * Mrf24j::get_rxbuf(void) {
	return rx_buf;
}

int Mrf24j::rx_datalength(void) {
	return rx_info.frame_length - bytes_nodata;
}

void Mrf24j::set_ignoreBytes(int ib) {
	// some modules behaviour
	ignoreBytes = ib;
}

/**
* Set bufPHY flag to buffer all bytes in PHY Payload, or not
*/
void Mrf24j::set_bufferPHY(boolean bp) {
	bufPHY = bp;
}

boolean Mrf24j::get_bufferPHY(void) {
	return bufPHY;
}

/**
* Set PA/LNA external control
*/
void Mrf24j::set_palna(boolean enabled) {
	if (enabled) {
		
		//Setup PA/LNA circuitry
		
		byte TRISGPIO = read_short(MRF_TRISGPIO);
		TRISGPIO |= 1 << 3; //Set GPIO3 as output;
		write_short(MRF_TRISGPIO,TRISGPIO);
		byte gpio = read_short(MRF_GPIO);
		gpio |= 1 << 3; //Enable PA power supply
		write_short(MRF_GPIO, gpio);
		
		write_long(MRF_TESTMODE, 0x07); // Set RF State machine into PA/LNA operation
		
		}else{
		write_long(MRF_TESTMODE, 0x00); // Set RF State machine into normal operation
	}
}

void Mrf24j::rx_flush(void) {
	write_short(MRF_RXFLUSH, 0x01);
}

void Mrf24j::rx_disable(void) {
	write_short(MRF_BBREG1, 0x04);  // RXDECINV - disable receiver
}

void Mrf24j::rx_enable(void) {
	write_short(MRF_BBREG1, 0x00);  // RXDECINV - enable receiver
}
