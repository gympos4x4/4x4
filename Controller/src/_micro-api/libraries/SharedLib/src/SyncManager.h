#pragma once

#ifndef __SYNC_MANAGER_h
#define __SYNC_MANAGER_h

#if !defined(TX_SYNC_COUNT)
#define TX_SYNC_COUNT -1
#endif

#if !defined(RX_SYNC_COUNT)
#define RX_SYNC_COUNT -1
#endif

#if TX_SYNC_COUNT < 0 || RX_SYNC_COUNT < 0
#error You have to define how many classes you want to sync
#endif

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
	static ISerializable *tx_sync[TX_SYNC_COUNT], *rx_sync[RX_SYNC_COUNT];
	static char tx_size;
	static char tx_buffer[117], *rx_buffer;
	static bool msg_came;

	SyncManager() = delete;
	~SyncManager() = delete;

public:
	static void setup(RadioConfig* _rc, Mrf24j* _mrf, ISerializable** _tx_sync, ISerializable** _rx_sync);
	static void loop();
	
private:
	static void serialize();	
	static void deserialize();
};

#endif