#include "utils.h"

void copyHex(uint8_t *d, uint16_t v) {
  d[1] = (uint8_t)v;
  d[0] = (uint8_t)(v >> 8);
}

void copyHex(uint8_t *d, uint32_t v) {
  copyHex(d, (uint16_t)(v >> 16));
  copyHex(d+2, (uint16_t)v);
}

void copyHexL(uint8_t *d, uint16_t v) {
  /* little endian */
  d[0] = (uint8_t)v;
  d[1] = (uint8_t)(v >> 8);
}

void copyHexL(uint8_t *d, uint32_t v) {
  /* little endian */
  copyHexL(d, (uint16_t)(v));
  copyHexL(d+2, (uint16_t)(v >> 16));
}
