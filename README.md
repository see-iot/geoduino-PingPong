# PingPong LoRa sample application

This is a simple point-to-point Ping-Pong sample application for Arduino-based LoRa devices.

The Ping program sends LoRa packets, and expects answers from the Pong program.

The Pong program waits passively for packets. Each time it decodes a packet, it sends a reply.

Each program displays RSSI values of each received packet.


## Requirements

Uses the [arduino-LoRa library](http://github.com/see-iot/arduino-LoRa).


## Compatible Hardware

Any hardware supported by the [arduino-LoRa library](http://github.com/see-iot/arduino-LoRa).

A LED should be connected to Pin 12 on the Ping device. The LED is powered on each time an answer is received.


## Build

1. Install the arduino-LoRa library into the Arduino IDE Library Manager.
2. Configure the radio parameters in the Ping.ino and the Pong.ino files.
3. Choose the target board (e.g. MKR-WAN-1300)
4. Build and load both the Ping.ino and the Pong.ino programs

When setting the radio parameters, be sure to also set the timing parameters to appropriate values, to leave some time for responses.

## Operation

As soon as the Ping device is powered on, it begins sending messages.

When the Pong device is powered on, it answers any received message from the Ping device.

The Ping device lights its LED each time a message is answered.

To evaluate the effective radio range, simply setup the Pong device at a fixed location, then move the Ping device and see how far messages can be exchanged by examining the LED.
