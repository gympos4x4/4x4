#include <SPI.h>
#include <mrf24j.h>

const int UNO_RESET      = 13;
const int UNO_CS         = 11;
const int UNO_INTERRUPT  = 2;

const int LEO_RESET      = 8;
const int LEO_CS         = 10;
const int LEO_INTERRUPT  = 3;

Mrf24j mrfUno(UNO_RESET, UNO_CS, UNO_INTERRUPT);
Mrf24j mrfLeo(LEO_RESET, LEO_CS, LEO_INTERRUPT);

unsigned long last_time = 0;
const long tx_interval = 1000;

const int LENGTH_TEST = 117;

void setup()
{
  Serial.begin(9600);
  Serial.println("PoC_radio");

  mrfUno.reset();
  mrfLeo.reset();

  mrfUno.init();
  mrfLeo.init();

  mrfUno.set_pan(0x1337);
  mrfLeo.set_pan(0x1337);

  mrfUno.address16_write(0x6000);
  mrfLeo.address16_write(0x6001);

  attachInterrupt(1, _int_uno, CHANGE);
  attachInterrupt(0, _int_leo, CHANGE);

  last_time = millis();
  interrupts();
}

void _int_uno(){
  mrfUno.interrupt_handler();
}

void _int_leo(){
  mrfLeo.interrupt_handler();
}

void loop()
{
  mrfUno.check_flags(&handle_rx, &handle_tx);
  unsigned long cur_time = millis();
  if(cur_time - last_time > tx_interval) {
    last_time = cur_time;
    Serial.println("txxxing...");
    
    char text[LENGTH_TEST];
    for(int i = 0; i < LENGTH_TEST; i++)
    {
      text[i] = 'A';
    }
    text[LENGTH_TEST - 1] = '\0';
    
    mrfLeo.send16(0x6000, text);
  }
}

void handle_rx() {
    Serial.print("received a packet ");Serial.print(mrfUno.get_rxinfo()->frame_length, DEC);Serial.println(" bytes long");
    
    if(mrfUno.get_bufferPHY()){
      Serial.println("Packet data (PHY Payload):");
      for (int i = 0; i < mrfUno.get_rxinfo()->frame_length; i++) {
          Serial.print(mrfUno.get_rxbuf()[i]);
      }
    }
    
    Serial.println("\r\nASCII data (relevant data):");
    for (int i = 0; i < mrfUno.rx_datalength(); i++) {
        Serial.write(mrfUno.get_rxinfo()->rx_data[i]);
    }
    
    Serial.print("\r\nLQI/RSSI=");
    Serial.print(mrfUno.get_rxinfo()->lqi, DEC);
    Serial.print("/");
    Serial.println(mrfUno.get_rxinfo()->rssi, DEC);
}

void handle_tx() {
    if (mrfLeo.get_txinfo()->tx_ok) {
        Serial.println("TX went ok, got ack\r\n\r\n");
    } else {
        Serial.print("TX failed after ");Serial.print(mrfLeo.get_txinfo()->retries);Serial.println(" retries\n");
    }
}
