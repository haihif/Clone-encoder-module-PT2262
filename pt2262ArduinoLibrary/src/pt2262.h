#ifndef PT2262_H
#define PT2262_H

//#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

void sendBit(uint8_t PT2262_BIT);

void sendFrame(uint8_t ADDRESS_RECEIVER_BITS, uint8_t dataCode);

uint8_t getState();

#ifdef __cplusplus
}
#endif

#endif
