#ifndef IbusNames_h
#define IbusNames_h

// IBUS module ID's
static const uint8_t M_GM5 = 0x00; // GM5: body control module
static const uint8_t M_DIA = 0x3F; // DIA: diagnostic computer
static const uint8_t M_EWS = 0x44; // EWS: immobilizer
static const uint8_t M_MFL = 0x50; // MFL: steering wheel controls
static const uint8_t M_IHKA = 0x5B; // IHKA: climate control panel
static const uint8_t M_RAD = 0x68; // RAD: radio module
static const uint8_t M_IKE = 0x80; // IKE: instrument cluster
static const uint8_t M_ALL = 0xBF; // ALL: broadcast message
static const uint8_t M_TEL = 0xC8; // TEL: telephone module
static const uint8_t M_LCM = 0xD0; // LCM: light control module

// GM5 input/output addresses
static const uint8_t GM5_BTN_DOME_LIGHT = 0x01; // dome light button
static const uint8_t GM5_BTN_CENTER_LOCK = 0x03; // center console lock/unlock button
static const uint8_t GM5_BTN_TRUNK_OPEN = 0x05; // interior trunk unlock button
static const uint8_t GM5_BTN_WINDOW_DRIVER_DOWN = 0x0A; // driver window down button
static const uint8_t GM5_BTN_WINDOW_DRIVER_UP = 0x0B; // driver window up button
static const uint8_t GM5_BTN_WINDOW_PASSENGER_DOWN = 0x0C; // passenger window down button
static const uint8_t GM5_BTN_WINDOW_PASSENGER_UP = 0x0D; // passenger window up button
static const uint8_t GM5_LED_ALARM_WARNING = 0x4E; // red LED under interior mirror ("clown nose")

// GM5 state groups
static const uint8_t GM5_INPUT_STATE_DIGITAL = 0x00; // request digital IO states
static const uint8_t GM5_INPUT_STATE_ANALOG = 0x01; // request analog IO states

#endif
