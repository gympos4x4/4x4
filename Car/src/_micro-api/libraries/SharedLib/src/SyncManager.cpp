#include "libs/mrf24j.h"
#include "SyncManager.h"
#include "WirelessConnection.h"
#include "ISerializable.h"

char SyncManager::tx_size;
bool SyncManager::msg_came;
char SyncManager::tx_buffer[117];
char* SyncManager::rx_buffer;
int SyncManager::tx_sync_count;
int SyncManager::rx_sync_count;
ISerializable** SyncManager::tx_sync;
ISerializable** SyncManager::rx_sync;

bool WirelessConnection::msg_came;

void SyncManager::setup(RadioConfig* _rc, Mrf24j* _mrf, ISerializable** _tx_sync, int _tx_sync_coumt, ISerializable** _rx_sync, int
                        _rx_sync_count)
{
	tx_size = 0;
	tx_sync_count = _tx_sync_coumt;
	rx_sync_count = _rx_sync_count;

	for (int i = 0; i < tx_sync_count; i++)
	{
		tx_sync[i] = _tx_sync[i];
		tx_size += tx_sync[i]->get_size();
	}

	for (int i = 0; i < rx_sync_count; i++)
	{
		rx_sync[i] = _rx_sync[i];
	}

	WirelessConnection::setup(_rc, _mrf, &SyncManager::rx_buffer, &SyncManager::msg_came);
}

void SyncManager::loop()
{
	serialize();
	WirelessConnection::send(tx_size, tx_buffer);

	int last = millis();
	while (!WirelessConnection::msg_came)
	{
		deserialize();
		WirelessConnection::msg_came = false;

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

	for (int i = 0; i < tx_sync_count; i++)
	{
		tx_sync[i]->serialize(tx_ptr);
		tx_ptr += tx_sync[i]->get_size();
	}
}

void SyncManager::deserialize()
{
	char* rx_ptr = rx_buffer;

	for (int i = 0; i < rx_sync_count; i++)
	{
		rx_sync[i]->deserialize(rx_ptr);
		rx_ptr += rx_sync[i]->get_size();
	}
}
