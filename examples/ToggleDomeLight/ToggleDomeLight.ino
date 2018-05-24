/*
example IBUS message:
50 04 68 32 11 1F (volume up button pressed on the steering wheel)
|  |  |  |  |  | 
|  |  |  |  |  checksum (xorsum of all previous bytes)
|  |  |  |  one or more data fields
|  |  |  message type/command type
|  |  destination address
|  length of message (including destination address and checksum)
source address 
*/

// this example shows how to transmit a message over the IBUS

#include <IbusTrx.h> // include the IbusTrx library

IbusTrx ibusTrx; // create a new IbusTrx instance

// define the message that we want to transmit
// the message must be defined as an array of uint8_t's (unsigned 8-bit integers)
uint8_t toggleDomeLight[6] = {
  M_DIA, // sender ID (diagnostic interface)
  0x05,  // length of the message payload (including destination ID and checksum)
  M_GM5, // destination ID (body control module)
  GM5_SET_IO, // the type of message (IO manipulation)
  GM5_BTN_DOME_LIGHT, // the first parameter (the IO line that we want to manipulate)
  0x01 // second parameter
  // don't worry about the checksum, the library automatically calculates it for you
};

void setup(){
  ibusTrx.begin(Serial); // begin listening for messages
}

void loop(){
  if (ibusTrx.available()) {    
    IbusMessage m = ibusTrx.readMessage(); // grab incoming messages

    // trigger based on a message from the steering wheel controls
    if (m.source() == M_MFL) {
      
      // if "talk" is held down for 1 second: simulate a press of the dome light button
      if (m.b(1) == 0x90) {
        // write the message to the transmit buffer
        ibusTrx.write(toggleDomeLight);

      }
    }
  }
  // remember to never use a blocking function like delay() in your program,
  // always use millis() or micros() if you have to implement a delay somewhere
}