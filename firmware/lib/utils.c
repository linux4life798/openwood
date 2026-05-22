#include "utils.h"

static volatile uint32_t *UtilsRegister32(uint32_t address)
{
    return (volatile uint32_t *)address;
}

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

uint32_t UtilsRead32(uint32_t address)
{
    return *UtilsRegister32(address);
}

void UtilsWrite32(uint32_t address, uint32_t value)
{
    *UtilsRegister32(address) = value;
}

void UtilsSetBits32(uint32_t address, uint32_t mask)
{
    UtilsWrite32(address, UtilsRead32(address) | mask);
}

void UtilsClearBits32(uint32_t address, uint32_t mask)
{
    UtilsWrite32(address, UtilsRead32(address) & ~mask);
}

void UtilsMaskedWrite32(uint32_t address, uint32_t mask, uint32_t value)
{
    UtilsWrite32(address, (UtilsRead32(address) & ~mask) | (value & mask));
}
