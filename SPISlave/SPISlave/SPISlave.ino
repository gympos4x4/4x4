/*
SPI Slave Demo Sketch
Connect the SPI Master device to the following pins on the esp8266:
GPIO    NodeMCU   Name  |   Uno
===================================
15       D8       SS   |   D10
13       D7      MOSI  |   D11
12       D6      MISO  |   D12
14       D5      SCK   |   D13
Note: If the ESP is booting at a moment when the SPI Master has the Select line HIGH (deselected)
the ESP8266 WILL FAIL to boot!
See SPISlave_SafeMaster example for possible workaround
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "SPISlave.h"

void setup_spi_slave()
{
  // data has been received from the master. Beware that len is always 32
  // and the buffer is autofilled with zeroes if data is less than 32 bytes long
  // It's up to the user to implement protocol for handling data length
  SPISlave.onData([](uint8_t * data, size_t len) {
    String message = String((char *)data);
    if(message.equals("Hello Slave!")) {
      SPISlave.setData("Hello Master!");
    } else if(message.equals("Are you alive?")) {
      char answer[33];
      sprintf(answer,"Alive for %u seconds!", millis() / 1000);
      SPISlave.setData(answer);
    } else {
      SPISlave.setData("Say what?");
    }
    Serial.printf("Question: %s\n", (char *)data);
  });

  // The master has read out outgoing data buffer
  // that buffer can be set with SPISlave.setData
  SPISlave.onDataSent([]() {
    Serial.println("Answer Sent");
  });

  // status has been received from the master.
  // The status register is a special register that bot the slave and the master can write to and read from.
  // Can be used to exchange small data or status information
  SPISlave.onStatus([](uint32_t data) {
    Serial.printf("Status: %u\n", data);
    SPISlave.setStatus(millis()); //set next status
  });

  // The master has read the status register
  SPISlave.onStatusSent([]() {
    Serial.println("Status Sent");
  });

  // Setup SPI Slave registers and pins
  SPISlave.begin();
}

const IPAddress ip_static(10,10,0,100);
const IPAddress ip_controller(10,10,0,1);
const IPAddress netmask(255,255,255,0);
const int udp_local_port = 1337;

WiFiUDP Udp;
char rx_buffer[255];

int8_t *throttle;
int8_t *steer;

void setup_wifi()
{
  WiFi.begin("ColumbaRacing", "MamkaMonika2017");
  WiFi.config(ip_static, ip_controller, netmask);
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Udp.begin(udp_local_port);

  Udp.beginPacket(ip_controller, 2017);
  Udp.write("HELO\n");
  Udp.endPacket();
}

void setup()
{
	Serial.begin(9600);
	Serial.setDebugOutput(true);	

  setup_spi_slave();
  setup_wifi();
}

void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    int len = Udp.read(rx_buffer, 255);    
    throttle = (int8_t*)(&rx_buffer[0]);
    steer = (int8_t*)(&rx_buffer[1]);
    SPISlave.setStatus( (*throttle << 8) | *steer );
    Serial.printf("Throttle: %d, Steering: %d\n", *throttle, *steer);
  }
}
