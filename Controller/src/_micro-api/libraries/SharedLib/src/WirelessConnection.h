#ifndef __WIRELESS_CONNECTION_h
#define __WIRELESS_CONNECTION_h

#include "RadioConfig.h"
#include "libs/mrf24j.h"

class WirelessConnection
{
	WirelessConnection() = delete;
	~WirelessConnection() = delete;

private:
	static RadioConfig *rc;
	static Mrf24j *mrf;
	static char** rx_buffer;

public:
	static bool msg_came;

public:
	static void setup(RadioConfig *rc, Mrf24j *mrf, char** rx_buffer, bool* _msg_came)
	{
		rc = rc;
		mrf = mrf;

		mrf->reset();
		mrf->init();

		mrf->set_pan(rc->pan);
		mrf->address16_write(rc->my_address);

		*rx_buffer = reinterpret_cast<char*>(mrf->get_rxinfo()->rx_data);
	}

	static void loop();

	static void send(int length, char* message);
	static void handle_rx();
	static void handle_tx();
};

#endif