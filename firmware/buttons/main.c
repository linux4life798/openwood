#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"
#include "rom_functions.h"
#include "rom_helpers.h"
#include "utils.h"

static void DisplayPressedKeysExample(void)
{
    char line[] = "K:-- -- -- --";
    uint8_t key_codes[BUTTON_MAX_PRESSED_KEYS];
    uint8_t key_count;
    uint8_t key_index;

    key_count = ButtonReadPressedKeys(key_codes, BUTTON_MAX_PRESSED_KEYS);
    for (key_index = 0; key_index < key_count; key_index++) {
        UtilsFormatHexByte(&line[2 + (key_index * 3)], key_codes[key_index]);
    }

    DisplayRenderText(line);
}

static void DisplayKeyEventExample(uint8_t key_code, bool released)
{
    char line[] = "P:--";

    if (released) {
        line[0] = 'R';
    }
    UtilsFormatHexByte(&line[2], key_code);

    DisplayRenderText(line);
}

/*
 * Show a prompt, then check for up to four keys that are currently held.
 * When a key is pressed or released, show that event briefly.
 */
int main(void)
{
    uint8_t key_code;
    bool released;

    CPUInitialize();
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
