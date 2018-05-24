/*
  IbusTrx (v2.4.0)
  Arduino library for sending and receiving messages over the BMW infotainment bus (IBUS).
  Author: D. van Gent
  More info: https://0x7b.nl/ibus

  THIS PROGRAM IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
*/

#include "IbusTrx.h"
// open serial port
void IbusTrx::begin(HardwareSerial &userPort) {
  serialPort = &userPort;
  serialPort->begin(9600, SERIAL_8E1);
}

// close serial port
void IbusTrx::end() {
  serialPort->end();
  clearBuffer();
}

// transmit and receive
// returns true if a valid IBUS message has been buffered
bool IbusTrx::available() {
  // clear old message before attempting to read new data
  if (rx_msg_waiting) {
    clearBuffer();
  }
  // prevent receive buffer from overflowing
  if (rx_bytes > 0xFE) {
    clearBuffer();
  }
  // discard buffer if time since last byte timer has expired
  if (rx_bytes > 0 && millis()-t_last_rx_byte >= 8) {
    clearBuffer();
  }
  // if data is available, buffer it up
  if (serialPort->available()) {
    uint8_t rx_byte = serialPort->read();    
    // ignore loopback bytes coming in right after transmitting
    if (tx_bytes > 0 && !tx_msg_waiting) {
      tx_bytes--;
    }
    // buffer incoming data
    else {
      rx_buffer[rx_bytes] = rx_byte;
      rx_bytes++;
    }
    t_last_rx_byte = millis();
  }
  // assume bus is clear for sending after a short period of inactivity
  if (tx_msg_waiting && millis()-t_last_rx_byte >= 32) {
    // send all bytes in the transmit buffer
    for (uint8_t b = 0; b < tx_bytes; b++) {
      serialPort->write(tx_buffer[b]);
    }    
    tx_msg_waiting = false; // clear tx wait flag
  }
  // check if the receive buffer contains a valid IBUS message
  if (checkMessage()) {    
    rx_msg_waiting = true; // set rx wait flag
    return true;
  }
  return false;
}

// returns true if the receive buffer contains a valid IBUS message
// example message: 80 05 bf 18 00 00 22 (sender, length, destination, data(*3), checksum)
bool IbusTrx::checkMessage() {
  // check if all bytes have been received
  if (rx_bytes > 4 && rx_bytes == rx_buffer[1]+2) {
    uint8_t sender = rx_buffer[0];
    uint8_t length = rx_buffer[1];
    uint8_t chksum = sender ^ length;
    for (uint16_t i = 2; i < length+1; i++) {
      chksum = chksum ^ rx_buffer[i];
    }
    if (rx_buffer[length+1] == chksum) {
      // checksums match, buffer contains a valid message
      return true;
    }
    else {
      // message received with invalid checksum: discard buffer
      clearBuffer();      
      return false;
    }
  }
  else {
    // message incomplete
    return false;
  }
}

// creates and returns an IbusMessage object containing the contents of the message
IbusMessage IbusTrx::readMessage() {
  IbusMessage ibusMessage(rx_buffer);
  clearBuffer(); // clear receive buffer after reading the message
  return ibusMessage;
}

// returns the number of bytes stored in the rx buffer
uint8_t IbusTrx::length() {
  return rx_bytes;
}

// reset receive buffer
void IbusTrx::clearBuffer() {
  rx_buffer[1] = 0x00;
  rx_bytes = 0;
  rx_msg_waiting = false;
}

// returns tx wait flag
bool IbusTrx::transmitWaiting() {
  return tx_msg_waiting;
}

// prepare message for transmission
void IbusTrx::write(uint8_t message[]) {
  // copy message to transmit buffer
  for (uint8_t p = 0; p <= message[1]; p++) {
    tx_buffer[p] = message[p];
  }
  // calculate checksum
  uint8_t chksum = tx_buffer[0] ^ tx_buffer[1];
  for (uint8_t i = 2; i < tx_buffer[1]+1; i++) {
    chksum = chksum ^ tx_buffer[i];
  }
  tx_buffer[tx_buffer[1]+1] = chksum;
  // set tx wait flag
  tx_msg_waiting = true;
  tx_bytes = tx_buffer[1] + 2;
}
