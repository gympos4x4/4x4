#pragma once

struct RadioConfig;
class Mrf24j;

class WirelessConnection
{
private:
	WirelessConnection();
	~WirelessConnection();

private:
	static RadioConfig *rc;
	static Mrf24j *mrf;
	static char** rx_buffer;
	static bool* msg_came;

public:
	static void setup(RadioConfig *rc, Mrf24j *mrf, char** rx_buffer, bool* _msg_came);
	static void loop();

	static void send(int length, char* message);
	static void handle_rx();
	static void handle_tx();
};