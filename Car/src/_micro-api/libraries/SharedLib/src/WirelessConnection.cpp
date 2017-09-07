#include "WirelessConnection.h"
#include "RadioConfig.h"
#include "libs/mrf24j.h"

/*
 *void tx_handler();
 *void rx_handler();
 */

RadioConfig* WirelessConnection::rc;
Mrf24j* WirelessConnection::mrf;
char** WirelessConnection::rx_buffer;
bool WirelessConnection::msg_came;

//void WirelessConnection::setup(RadioConfig* rc, Mrf24j* mrf, char** rx_buffer, bool* _msg_came)
//{
//	rc = rc;
//	mrf = mrf;	
//	msg_came = _msg_came;
//
//	mrf->reset();
//	mrf->init();
//
//	mrf->set_pan(rc->pan);
//	mrf->address16_write(rc->my_address);
//
//	*rx_buffer = reinterpret_cast<char*>(mrf->get_rxinfo()->rx_data);
//}

void WirelessConnection::loop()
{
	mrf->check_flags(handle_rx, handle_tx);
}

void WirelessConnection::send(int length, char* message)
{
	mrf->send16(rc->buddy_address, length, message);
}

void WirelessConnection::handle_rx()
{
	msg_came = true;
}

void WirelessConnection::handle_tx()
{
	if(!mrf->get_txinfo()->tx_ok)
	{
		Serial.print("TX failed after ");
		Serial.print(mrf->get_txinfo()->retries);
		Serial.println(" retries.");
	}
}