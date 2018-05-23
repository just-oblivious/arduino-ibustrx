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

// include the IbusTrx library
#include <IbusTrx.h>

// create a new IbusTrx instance
IbusTrx ibusTrx;

void setup(){
  // begin listening for IBUS messages on the hardware serial port
  // note: this library is hard coded to the first hardware serial port
  // software-based serial ports are absolutely not recommended for this
  ibusTrx.begin();
  pinMode(13, OUTPUT);
}
void loop(){
  // transceive() has to be called repeatedly, with no delay() in between
  // this function buffers the incoming bytes and transmits anything waiting in the transmit queue 
  // note: messages are only transmitted when the bus is clear
  // returns true if a valid message has been buffered
  bool messageWaiting = ibusTrx.transceive();

  // if there's a message waiting, check it out
  if (messageWaiting) {
    // grab incoming message from buffer (this copies the message and clears the receive buffer)
    IbusMessage message = ibusTrx.message();

    // every module on the IBUS has its own 8-bit address.
    // the following addresses are defined in the Ibustrx library:
    // M_GM5: body control module
    // M_DIA: diagnostic computer
    // M_EWS: immobilizer
    // M_MFL: steering wheel controls
    // M_IHKA: climate control panel
    // M_RAD: radio module
    // M_IKE: instrument cluster
    // M_ALL: broadcast message
    // M_TEL: telephone module
    // M_LCM: light control module
    
    // the source() and destination() functions return the addresses
    unsigned int messageSource = message.source();
    unsigned int messageDestination = message.destination();

    // filtering example: 
    // (only read messages sent by the steering to the radio)
    if (messageSource == M_MFL && messageDestination == M_RAD) {
      // the length of the message payload, including the checksum
      // this function is rarely needed, in most cases it's already known how many fields there are based on the message type
      unsigned int messageLength = message.length();

      // the b(n) function returns the n'th byte of the message payload
      // b(0) will return the first byte, b(1) the second, etc.
      unsigned int messageCommand = message.b(0); // the first byte usually identifies what type of message it is
      
      // command 0x32 from the steering wheel happens to be related to the volume controls
      if (messageCommand == 0x32) {
        // in this case, the least significant bit of the second byte tells us whether this is a "volume up" or a "volume down" instruction
        // several fields are often packed into a single byte, the use of bitwise logic is recommended when working with IBUS data
        if (message.b(1) & 0x01){
          // volume up pressed, turn LED on
          digitalWrite(13, HIGH);
        }
        else{
          // volume down pressed, turn LED off
          digitalWrite(13, LOW);
        }
      }
      // etc.
    }
    // etc.
  }
  // rest of your program goes here...

  // remember to never use a blocking function like delay() in your program,
  // always use millis() or micros() if you have to implement an interval somewhere
}