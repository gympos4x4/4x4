#include <assert.h>
#include "SyncManager.h"

SyncManager::SyncManager(size_t count, ISerializable ** classes, RadioConfig rc) : rf_radio(rc.pin_layout.pin_reset, rc.pin_layout.pin_cs, rc.pin_layout.pin_interrupt), radio_config(rc)
{
	assert(classes != nullptr);
	assert(count < 116 && count > 0);

	if (!Serial)
	{
		Serial.begin(SERIAL_BAUD_RATE);
	}

	list_length = count;

	/* Initialize internal buffers */
	buffer_size = 0;

	synced_classes = static_cast<ISerializable**>(calloc(count, sizeof(ISerializable*)));
	synced_classes_offsets = static_cast<uint16_t*>(calloc(count, sizeof(uint16_t)));

	for (uint8_t i = 0; i < count; i++)
	{
		assert(classes[i] != nullptr);

		synced_classes[i] = classes[i];
		synced_classes_offsets[i] = buffer_size;
		buffer_size += synced_classes[i]->get_size();
	}

	r_buffer = static_cast<char*>(malloc(buffer_size)); //reinterpret_cast<char*>(rf_radio.get_rxinfo()->rx_data);
	t_buffer = static_cast<char*>(malloc(buffer_size));

	/* Initialize radio library */
	rf_radio.reset();
	rf_radio.init();

	rf_radio.set_pan(rc.panid);
	rf_radio.address16_write(rc.my_address);

	last_time = millis();
}

SyncManager::~SyncManager()
{
	free(r_buffer);
	free(t_buffer);
}

void SyncManager::Sync()
{
	EncodeMessage(t_buffer);

	rf_radio.send16(radio_config.buddy_address, t_buffer);
}

void SyncManager::EncodeMessage(char* buffer) const
{
	for (int i = 0; i < list_length; i++)
	{
		synced_classes[i]->serialize(get_buffer_offset(buffer, i));
	}
}

void SyncManager::DecodeMessage(char* buffer) const
{
	for (int i = 0; i < list_length; i++)
	{
		synced_classes[i]->deserialize(get_buffer_offset(buffer, i));
	}
}

void SyncManager::RecieveMessageHandler()
{
	DecodeMessage((char*)rf_radio.get_rxinfo()->rx_data);
}

void SyncManager::TransmitMessageHandler()
{
	if (!rf_radio.get_txinfo()->tx_ok)
	{
		Serial.write("Failed to send after ");
		Serial.print(rf_radio.get_txinfo()->retries);
		Serial.println(" retries\n");
	}
	else
	{
		//asi netreba robit nic, posle sa o chvilu dalsi
	}
}

void SyncManager::interrupt_routine()
{
	rf_radio.interrupt_handler();
}

void SyncManager::loop()
{
	rf_radio.check_flags_custom(&SyncManager::RecieveMessageHandler, &SyncManager::TransmitMessageHandler, this);

	uint64_t current_time = millis();
	if(current_time - last_time <= send_delay)
	{
		last_time = current_time;

		Sync();
	}
}
