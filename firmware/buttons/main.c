#include <stdint.h>
#include <stdbool.h>

#include "rom_functions.h"
#include "rom_helpers.h"

static char HexDigit(uint8_t value)
{
    value &= 0x0fu;
    if (value < 10) {
        return '0' + value;
    }

    return 'A' + (value - 10);
}

static void FormatHexByte(char *dst, uint8_t value)
{
    dst[0] = HexDigit(value >> 4);
    dst[1] = HexDigit(value);
}

static void DisplayPressedKeysExample(void)
{
    char line[] = "K:-- -- -- --";
    uint8_t key_codes[BUTTON_MAX_PRESSED_KEYS];
    uint8_t key_count;
    uint8_t key_index;

    key_count = ButtonReadPressedKeys(key_codes, BUTTON_MAX_PRESSED_KEYS);
    for (key_index = 0; key_index < key_count; key_index++) {
        FormatHexByte(&line[2 + (key_index * 3)], key_codes[key_index]);
    }

    DisplayRenderText(line);
}

static void DisplayKeyEventExample(uint8_t key_code, bool released)
{
    char line[] = "P:--";

    if (released) {
        line[0] = 'R';
    }
    FormatHexByte(&line[2], key_code);

    DisplayRenderText(line);
}

int main(void)
{
    uint8_t key_code;
    bool released;

    InitializePeripherals();
    DisplayRenderText("Press keys");
    TimerDelayMilliseconds(500);

    while (1) {
        DisplayPressedKeysExample();

        if (ButtonReadKeyEvent(&key_code, &released)) {
            DisplayKeyEventExample(key_code, released);
            TimerDelayMilliseconds(300);
        }

        TimerDelayMilliseconds(100);
    }
}
