#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/**
 * Converts the low nibble of value into an uppercase hexadecimal digit.
 */
char UtilsHexDigit(uint8_t value);

/**
 * Formats value as two uppercase hexadecimal digits into dst.
 */
void UtilsFormatHexByte(char *dst, uint8_t value);

/**
 * Formats value as eight uppercase hexadecimal digits into dst.
 */
void UtilsFormatHexWord(char *dst, uint32_t value);

#endif // UTILS_H
