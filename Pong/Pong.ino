/* This example shows how to use the SX1272 chip
 * (part of Murata module) without any external stack.
 * To achieve this, we must setup the modem in dumb mode
 * and use LORA_IRQ_DUMB pin as chip select and SPI1 as communication port.
 * 
 * The example is based on LoraSender by @sandeepmistry arduino-LoRa library
 * https://github.com/sandeepmistry/arduino-LoRa
 * 
 * Since there is no IRQ pin available the host must poll for data (unfortunately)
 * 
 */

#include <SPI.h>
#include <LoRa.h>
#include <MKRWAN.h>

int counter = 0;

LoRaModem modem;

void setup() {
  Serial.begin(9600);
  //while (!Serial);

  modem.dumb();

  Serial.println("LoRa Sender");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(LORA_IRQ_DUMB, 6, 1); // set CS, reset, IRQ pin
  LoRa.setSPIFrequency(100000);

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSpreadingFactor(7);
  //LoRa.setSignalBandwidth(31.25E3);
  LoRa.enableCrc();
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet of size ");
    Serial.print(packetSize);
    Serial.print(": '");

    static char received[100];
    strcpy(received, "PONG ");
    
    int nb_received = strlen(received);
  
    // read packet
    while (LoRa.available()) {
      int by = LoRa.read();
      if (by != -1) {
        if (nb_received < sizeof(received) - 1) {
          received[nb_received++] = (char) by;
        }
      }
    }
    received[nb_received] = 0;

    while (LoRa.parsePacket()) {};
    Serial.print(received + 5);

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    // Send back message
    LoRa.beginPacket();
    LoRa.print(received);
    LoRa.endPacket();
  }
}
