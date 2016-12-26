/*
 Name:		Messaging.ino
 Created:	12/25/16 6:12:46 PM
 Author:	Matej
*/

#include "ISerializable.h"
#include "SyncManager.h"

//Priklad ako vyzeraju triedy, ak ich chceme synchronizovat:
//vsetky musia verejne dedit ISerializable (formulka ": public ISerializable")
class Servo : public ISerializable
{
private:
	//tuto premennu chceme mat zosynchronizovanu s druhym zariadenim
	//nemusi byt public aby sme to mohli urobit
	uint8_t uhol;

public:
	// vrati kolko byte-ov potrebujeme v bufferi
	virtual int get_size() const override
	{
		//sizeof(VAR) vrati velkost premennej VAR v byte-och
		return sizeof(uhol);
	}

	virtual void serialize(char* buffer) const override
	{
		assert(buffer != nullptr);

		memcpy(buffer, &uhol, get_size());
	}

	// Inherited via ISerializable
	virtual void deserialize(char * buffer) override
	{
		assert(buffer != nullptr);

		memcpy(&uhol, buffer, get_size());
	}
};

class Senzor : public ISerializable
{
public:
	//tieto premenne chceme synchronizovat
	float vzdialenost;

	//pole musi byt staticke, inac to nebude fungovat
	//dalo by sa dorobit aj dynamicke, ak by bolo treba
	float pole[10];

public:
	// Inherited via ISerializable
	virtual int get_size() const override
	{
		return sizeof(vzdialenost) + sizeof(pole);
	}

	virtual void serialize(char* buffer) const override
	{
		memcpy(buffer, &vzdialenost, get_size());
	}

	virtual void deserialize(char* buffer) override
	{
		memcpy(&vzdialenost, buffer, get_size());
	}
};

SyncManager *sync_manager;

ISerializable* to_sync[3];

// the setup function runs once when you press reset or power the board
void setup() {
	Servo predne, zadne;
	Senzor senzor;

	to_sync[0] = &predne;
	to_sync[1] = &zadne;
	to_sync[2] = &senzor;

	RadioConfig radio;
	radio.my_address = 0x1;
	radio.buddy_address = 0x2;
	radio.panid = 0x1337;

	sync_manager = new SyncManager(3, to_sync, radio);

	attachInterrupt(0, interrupt_routine, CHANGE);
	interrupts();
}

// the loop function runs over and over again until power down or reset
void loop() {
	sync_manager->loop();
}

void interrupt_routine()
{
	sync_manager->interrupt_routine();
}