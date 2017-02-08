#ifndef _UTILS_H_
#define _UTILS_H_

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


void copyHex(uint8_t *d, uint16_t v);
void copyHex(uint8_t *d, uint32_t v);
void copyHexL(uint8_t *d, uint16_t v);
void copyHexL(uint8_t *d, uint32_t v);

#endif

