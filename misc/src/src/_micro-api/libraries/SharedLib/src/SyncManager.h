// SyncManager.h

#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ISerializable.h"
#include "libs/mrf24j.h"

//Stores configuration for RF module
struct RadioConfig
{
	word my_address;
	word buddy_address;
	word panid;

	struct
	{
		int pin_reset = 6;
		int pin_cs = 10;
		int pin_interrupt = 2;
	} pin_layout;
};

//Main class for handling communivation of 2 devices
//TODO: Optimise library for MTF24J - enable amplification
class SyncManager
{
private:
	const int SERIAL_BAUD_RATE = 9600;

	char* r_buffer = nullptr;		//recieve buffer
	char* t_buffer = nullptr;		//transmit buffer

	size_t list_length, buffer_size;
	ISerializable** synced_classes;	//instances of all classes that are to be synced across devices 
	uint16_t* synced_classes_offsets;

	//time between updating data
	const uint16_t send_delay = 100;
	uint64_t last_time = 0;

	Mrf24j rf_radio;
	RadioConfig radio_config;

public:
	explicit SyncManager(size_t count, ISerializable** classes, RadioConfig rc);
	SyncManager() = delete;
	~SyncManager();

	//call this interrupt routine - handles all stuff for communication
	void interrupt_routine();

	//call at start of main arduino loop - handles exchange of information
	//WARNING: So far doesn't guarantee error-free communication. 
	void loop();

private:
	void Sync();

	void EncodeMessage(char* buffer) const;
	void DecodeMessage(char* buffer) const;

	void RecieveMessageHandler();
	void TransmitMessageHandler();

	char* get_buffer_offset(char* buffer, int class_index) const { return buffer + synced_classes_offsets[class_index]; };
};

