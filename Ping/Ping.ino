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

const int LED         = 12;
const int SF          = 12;
const float BW        = 125E3;
const bool CRC        = true;
const float FREQ      = 868E6;
const long RX_TIMEOUT = 10000;
const long TX_DELAY   = 1500;

int counter = 0;

LoRaModem modem;

void setup() {
  pinMode(LED, OUTPUT);
  
  Serial.begin(9600);
  //while (!Serial);

  modem.dumb();

  Serial.println("LoRa Sender");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(LORA_IRQ_DUMB, 6, 1); // set CS, reset, IRQ pin
  LoRa.setSPIFrequency(100000);

  if (!LoRa.begin(FREQ)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setTxPower(17, PA_OUTPUT_PA_BOOST_PIN);
  LoRa.setSpreadingFactor(SF);
  LoRa.setSignalBandwidth(BW);
  if (CRC) {
    LoRa.enableCrc();
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("PING ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  int timeout = millis() + RX_TIMEOUT;
  int packetSize;

  // try to parse packet
  do {
    packetSize = LoRa.parsePacket();
    if (packetSize) {
      digitalWrite(LED, HIGH);
      // received a packet
      Serial.print("Received packet '");
  
      static char received[100];
      int nb_received = 0;
    
      // read packet
      while (LoRa.available() && (nb_received < sizeof(received) - 1)) {
        received[nb_received++] = (char)LoRa.read();
      }
      received[nb_received] = 0;
      // parsePacket still returns 
      while (LoRa.parsePacket()) {};
      Serial.print(received);
  
      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
    }
  } while (!packetSize && (millis() < timeout));
  // Just wait a bit before sending next packet
  delay(TX_DELAY);

  digitalWrite(LED, LOW);
}

