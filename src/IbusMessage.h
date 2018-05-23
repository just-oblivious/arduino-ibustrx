#ifndef IbusMessage_h
#define IbusMessage_h

#include "Arduino.h"

class IbusMessage{
  public:
    uint8_t source();
    uint8_t length();
    uint8_t destination();
    uint8_t b(uint8_t n);
    IbusMessage(uint8_t *buffer);
    ~IbusMessage();

  private:
    uint8_t *m_buffer; // temporary message buffer
};
#endif
