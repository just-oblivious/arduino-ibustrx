// this example shows how to send a basic message

#include <IbusTrx.h> // include the IbusTrx library

IbusTrx ibusTrx; // create a new IbusTrx instance

void setup(){
  ibusTrx.begin(); // begin listening for messages
}

void loop(){
  if (ibusTrx.transceive()) {    
    IbusMessage m = ibusTrx.message(); // grab incoming messages

    // trigger based on a message from the steering wheel controls
    if (m.source() == M_MFL) {
      // if "talk" is held down for 1 second: simulate a press of the dome light button
      if (m.b(1) == 0x90) {
        // send a diagnostic message to the body control module
        ibusTrx.send(new uint8_t[6] {M_DIA, 0x05, M_GM5, 0x0C, GM5_BTN_DOME_LIGHT, 0x01});
      }
    }
  }
  // remember to never use a blocking function like delay() in your program,
  // always use millis() or micros() if you have to implement an interval somewhere
}