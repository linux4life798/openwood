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

/**
 * Reads a 32-bit memory-mapped register.
 */
uint32_t UtilsRead32(uint32_t address);

/**
 * Writes a 32-bit memory-mapped register.
 */
void UtilsWrite32(uint32_t address, uint32_t value);

/**
 * Sets bits in a 32-bit memory-mapped register.
 */
void UtilsSetBits32(uint32_t address, uint32_t mask);

/**
 * Clears bits in a 32-bit memory-mapped register.
 */
void UtilsClearBits32(uint32_t address, uint32_t mask);

/**
 * Replaces masked bits in a 32-bit memory-mapped register.
 */
void UtilsMaskedWrite32(uint32_t address, uint32_t mask, uint32_t value);

#endif // UTILS_H
