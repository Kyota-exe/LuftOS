#ifndef LUFTOS_STRINGCONVERSION_H
#define LUFTOS_STRINGCONVERSION_H

#include <stdint.h>

const char* ToString(uint64_t value);
const char* ToString(int64_t value);
const char* ToString(double value, uint8_t decimalPlaces);

#endif
