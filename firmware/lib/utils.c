#include "utils.h"

char UtilsHexDigit(uint8_t value)
{
    value &= 0x0fu;
    if (value < 10) {
        return '0' + value;
    }

    return 'A' + (value - 10);
}

void UtilsFormatHexByte(char *dst, uint8_t value)
{
    dst[0] = UtilsHexDigit(value >> 4);
    dst[1] = UtilsHexDigit(value);
}

void UtilsFormatHexWord(char *dst, uint32_t value)
{
    uint8_t digit_index;

    for (digit_index = 0; digit_index < 8; digit_index++) {
        dst[digit_index] = UtilsHexDigit(value >> (28 - (digit_index * 4)));
    }
}
