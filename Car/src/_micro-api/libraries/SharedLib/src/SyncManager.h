#ifndef __SYNC_MANAGER_h
#define __SYNC_MANAGER_h

//how much ms to wait before skipping current data
#define WAIT_FOR_SYNC 0
//sync interval in miliseconds
#define SYNC_INTERVAL 0

class Mrf24j;
class RadioConfig;
class ISerializable;

class SyncManager
{
private:
	static ISerializable **tx_sync, **rx_sync;
	static int tx_sync_count, rx_sync_count;
	static char tx_size;
	static char tx_buffer[117], *rx_buffer;
	static bool msg_came;

	SyncManager() = delete;
	~SyncManager() = delete;

public:
	static void setup(RadioConfig* _rc, Mrf24j* _mrf, ISerializable** _tx_sync, int _tx_sync_coumt, ISerializable** _rx_sync, int _rx_sync_count);
	static void loop();

private:
	static void serialize();
	static void deserialize();
};

#endif
