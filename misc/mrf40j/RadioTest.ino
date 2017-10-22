#include "SharedLib.h"

SyncManager smUno, smLeo;

TestSensor *sendSensor, *recvSensor;

void setup() {
    RadioConfig rcUno, rcLeo;

    rcUno.panid = rcLeo.panid = 0x1337;
    rcUno.my_address = rcLeo.buddy_address = 0x6000;
    rcLeo.my_address = rcUno.buddy_address = 0x6001;

    rcUno.pin_layout.pin_reset = 13;
    rcUno.pin_layout.pin_cs = 11;
    rcUno.pin_layout.interrupt = 1;

    rcLeo.pin_layout.pin_reset = 8;
    rcLeo.pin_layout.pin_cs = 10;
    rcLeo.pin_layout.interrupt = 0;

    sendSensor = new TestSensor(true);
    recvSensor = new TestSensor(false);

    smUno = SyncManager(1, &sendSensor, rcUno);
    smLeo = SyncManager(1, &recvSensor, rcLeo);
}

void loop() {
    smUno.loop();
    smLeo.loop();
}
