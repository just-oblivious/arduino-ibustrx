// this example shows how to interpret all messages related to the steering wheel controls

#include <IbusTrx.h> // include the IbusTrx library

IbusTrx ibusTrx; // create a new IbusTrx instance

void setup(){
  ibusTrx.begin(); // begin listening for messages
}

void loop(){
  if (ibusTrx.transceive()) {    
    IbusMessage m = ibusTrx.message(); // grab incoming messages

    if (m.source() == M_MFL) { 
      switch (m.b(0)) {
        case 0x32: // volume controls
          if (m.b(1) & 0x01) {
            // volume up pressed
          }
          else {
            // volume down pressed
          }
          break;
        case 0x3B: // other controls
          if (m.b(1) & 0x10) {
            if (m.b(1) & 0x01) { 
              // track up (held down for 1 second)
            }
            else if (m.b(1) & 0x08) {
              // track down (held down for 1 second)
            }
            else if (m.b(1) & 0x80) {
              // talk (held down for 1 second)
            }
          }
          else if (m.b(1) & 0x20) { 
            if (m.b(1) & 0x01) {
              // track up (released)
            }
            else if (m.b(1) & 0x08) {
              // track down (released)
            }
            else if (m.b(1) & 0x80) {
              // talk (released)
            }
          }
          else{
            if (m.b(1) & 0x01) {
              // track up (pressed)
            }
            else if (m.b(1) & 0x08) {
              // track down (pressed)
            }
            else if (m.b(1) & 0x80) {
              // talk (pressed)
            }
          }
          break;
      }
    }
  }
  // remember to never use a blocking function like delay() in your program,
  // always use millis() or micros() if you have to implement an interval somewhere
}