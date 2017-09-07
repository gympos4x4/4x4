#include "libs/mrf24j.h"
#include "SyncManager.h"
#include "WirelessConnection.h"
#include "ISerializable.h"


void SyncManager::setup(RadioConfig* _rc, Mrf24j* _mrf, ISerializable** _tx_sync, ISerializable** _rx_sync)
{
	tx_size = 0;

	for(int i = 0; i < TX_SYNC_COUNT; i++)
	{
		tx_sync[i] = _tx_sync[i];
		tx_size += tx_sync[i]->get_size();
	}

	for (int i = 0; i < RX_SYNC_COUNT; i++)
	{
		rx_sync[i] = _rx_sync[i];
	}

	WirelessConnection::setup(_rc, _mrf, &rx_buffer, &msg_came);
}

void SyncManager::loop()
{
	serialize();
	WirelessConnection::send(tx_size, tx_buffer);

	int last = millis();
	while(!msg_came)
	{
		deserialize();
		msg_came = false;

		const int now = millis();
		if (last - now > WAIT_FOR_SYNC)
		{
			break;
		}
		last = now;
	}
}

void SyncManager::serialize()
{
	char* tx_ptr = tx_buffer;

	for(int i = 0; i < TX_SYNC_COUNT; i++)
	{
		tx_sync[i]->serialize(tx_ptr);
		tx_ptr += tx_sync[i]->get_size();
	}
}

void SyncManager::deserialize()
{
	char* rx_ptr = rx_buffer;

	for(int i = 0; i < RX_SYNC_COUNT; i++)
	{
		rx_sync[i]->deserialize(rx_ptr);
		rx_ptr += rx_sync[i]->get_size();
	}
}
