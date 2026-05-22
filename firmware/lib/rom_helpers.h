#ifndef ROM_HELPERS_H
#define ROM_HELPERS_H

#include <stdbool.h>
#include <stdint.h>

#define BUTTON_KEY_INVALID 0xffu
#define BUTTON_MAX_PRESSED_KEYS 4u

/**
 * Initializes the ROM boot peripherals used by this library.
 *
 * Starts timer3, applies the ROM pinmux tables, initializes and clears the LCD
 * panel, initializes key-scanner and display-support control over I2C0
 * (likely including LCD backlight control), enables the LCD controller, and
 * selects the ROM boot text layout.
 */
void InitializePeripherals(void);
void DisplayClear(void);
void DisplayRenderText(const char *text);

/**
 * Returns 1 when the boot ROM's programming-mode button condition is active.
 *
 * Call InitializePeripherals() before this so key-scanner support is ready.
 */
bool ButtonCheckProgrammingModePressed(void);

/**
 * Returns true when the PTT button GPIO input is active.
 *
 * The PTT GPIO is active-low, so a cleared input bit means pressed.
 */
bool ButtonCheckPttPressed(void);

/**
 * Decodes a raw key byte to a logical key code.
 *
 * The low seven bits are decoded through the key map. Returns
 * BUTTON_KEY_INVALID when the raw key is empty or unmapped.
 */
uint8_t ButtonDecodeRawKey(uint8_t raw_key);

/**
 * Reads the key-scanner single-key event register and decodes it.
 *
 * This reads one event from the key-scanner fifo, which will be a single key
 * pressed or released event. If the user presses multiple keys in quick
 * succession, continue polling this function to receive each one in order.
 *
 * Call InitializePeripherals() before this so key-scanner support is ready.
 * Returns true when a mapped key event is available. When released_out is not NULL,
 * it receives bit 7 of the raw event byte: false for press, true for release.
 */
bool ButtonReadKeyEvent(uint8_t *key_code_out, bool *released_out);

/**
 * Reads and decodes up to four **currently** pressed keys.
 *
 * Returns only four keys that are being actively held down. You could use this
 * to poll for a certain set of keys being held down together.
 * Use ButtonReadKeyEvent to get the edge triggered pressed and released events.
 *
 * Call InitializePeripherals() before this so key-scanner support is ready.
 * Returns the number of decoded logical key codes stored in key_codes.
 */
uint8_t ButtonReadPressedKeys(uint8_t *key_codes, uint8_t max_key_codes);

#endif // ROM_HELPERS_H
