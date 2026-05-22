#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "rom_helpers.h"
#include "rom_functions.h"

typedef struct {
    uint16_t left;
    uint16_t top;
    uint16_t width;
    uint16_t height;
    uint8_t max_chars;
    uint8_t font_size_code;
    uint16_t reserved;
} BootDisplayLayout;

#define BOOT_DISPLAY_MODE ((volatile uint8_t *)0xffff01cbu)
#define BOOT_DISPLAY_LAYOUTS ((const BootDisplayLayout *)0x6000db80u)

#define GPIO_BANK01_INPUT_DATA ((volatile uint32_t *)0x01e26020u)
#define PTT_BUTTON_GPIO_MASK (1u << 7)

#define BUTTON_I2C_DEVICE_ADDRESS 0x44u
#define BUTTON_KEY_EVENT_REGISTER 0x10u
#define BUTTON_KEY_FIFO_REGISTER 0x0bu
#define BUTTON_RAW_KEY_MASK 0x7fu
#define BUTTON_RELEASE_FLAG 0x80u
#define BUTTON_KEY_MAP_MAX_RAW 0x70u

static const uint8_t button_key_map[BUTTON_KEY_MAP_MAX_RAW + 1] = {
    0x14, 0x11, 0x0e, 0x0b, 0x04, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x0a, 0x12, 0x0f, 0x0c, 0x09, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x15, 0x13, 0x10, 0x0d, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x03, 0x02, 0x01, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x08, 0x07, 0x06, 0x05, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff
};

static const BootDisplayLayout *DisplayGetCurrentLayout(void)
{
    return &BOOT_DISPLAY_LAYOUTS[*BOOT_DISPLAY_MODE];
}

static void DisplayBuildBlitDescriptors(LcdBlitFrame *frame, LcdWindowRect *window)
{
    const BootDisplayLayout *layout = DisplayGetCurrentLayout();

    frame->pixels = DisplayFramebufferGetPixelAddress(0, 0);
    frame->width = layout->width;
    frame->height = layout->height;
    frame->pixel_offset = 0;
    frame->flags = 0;
    frame->reserved0 = 0;
    frame->reserved1 = 0;
    frame->reserved2 = 0;

    window->left = layout->left;
    window->top = layout->top;
    window->width = layout->width;
    window->height = layout->height;
}

void InitializePeripherals(void)
{
    TimerInitialize(3, 0);
    SystemApplyPinmuxTables();
    DisplayInitializePanelAndClear();
    DisplayInitializeIoExpanderSupport();
    LcdControllerSetDisplayEnableFlag(1);
    DisplaySelectBootLayoutAndResetTextState();
}

void DisplayClear(void)
{
    LcdControllerFillScreen(0);
}

void DisplayRenderText(const char *text)
{
    LcdBlitFrame frame;
    LcdWindowRect window;
    const BootDisplayLayout *layout;
    uint32_t pixel_count;
    uint32_t pixel_index;
    uint32_t column;
    uint16_t char_count;
    uint8_t glyph_width;

    if (text == NULL) {
        text = "";
    }

    DisplayBuildBlitDescriptors(&frame, &window);
    if (frame.pixels == 0) {
        return;
    }

    pixel_count = (uint32_t)frame.width * (uint32_t)frame.height;
    for (pixel_index = 0; pixel_index < pixel_count; pixel_index++) {
        frame.pixels[pixel_index] = 0;
    }

    layout = DisplayGetCurrentLayout();
    column = 0;
    char_count = 0;
    while ((char_count < layout->max_chars) && (*text != '\0')) {
        glyph_width = DisplayDrawGlyphToFramebuffer((uint8_t *)text, column & 0xffffu, 0);
        column += glyph_width;
        text++;
        char_count++;
    }

    DisplayBlitRegion(&frame, &window);
}

bool ButtonCheckProgrammingModePressed(void)
{
    uint32_t program_key_pressed = BootKeyCheckProgramModeKey();

    if (ButtonCheckPttPressed() && (program_key_pressed == 1)) {
        return 1;
    }

    return 0;
}

bool ButtonCheckPttPressed(void)
{
    uint32_t gpio_bank01_input = *GPIO_BANK01_INPUT_DATA;

    return ((gpio_bank01_input & PTT_BUTTON_GPIO_MASK) == 0);
}

uint8_t ButtonDecodeRawKey(uint8_t raw_key)
{
    raw_key &= BUTTON_RAW_KEY_MASK;
    if (raw_key > BUTTON_KEY_MAP_MAX_RAW) {
        return BUTTON_KEY_INVALID;
    }

    return button_key_map[raw_key];
}

bool ButtonReadKeyEvent(uint8_t *key_code_out, bool *released_out)
{
    uint8_t raw_event = 0;
    uint8_t key_code;

    if (I2CReadRegisterBytes(BUTTON_I2C_DEVICE_ADDRESS, BUTTON_KEY_EVENT_REGISTER, &raw_event,
                             1) != 0) {
        return false;
    }

    key_code = ButtonDecodeRawKey(raw_event);
    if (key_code == BUTTON_KEY_INVALID) {
        return false;
    }

    if (key_code_out != NULL) {
        *key_code_out = key_code;
    }
    if (released_out != NULL) {
        *released_out = ((raw_event & BUTTON_RELEASE_FLAG) != 0);
    }

    return true;
}

uint8_t ButtonReadPressedKeys(uint8_t *key_codes, uint8_t max_key_codes)
{
    uint8_t raw_keys[BUTTON_MAX_PRESSED_KEYS];
    uint8_t raw_key_index;
    uint8_t key_code_count = 0;

    if ((key_codes == NULL) || (max_key_codes == 0)) {
        return 0;
    }

    if (I2CReadRegisterBytes(BUTTON_I2C_DEVICE_ADDRESS, BUTTON_KEY_FIFO_REGISTER, raw_keys,
                             BUTTON_MAX_PRESSED_KEYS) != 0) {
        return 0;
    }

    for (raw_key_index = 0; raw_key_index < BUTTON_MAX_PRESSED_KEYS; raw_key_index++) {
        uint8_t key_code = ButtonDecodeRawKey(raw_keys[raw_key_index]);

        if (key_code == BUTTON_KEY_INVALID) {
            continue;
        }

        key_codes[key_code_count] = key_code;
        key_code_count++;
        if (key_code_count == max_key_codes) {
            break;
        }
    }

    return key_code_count;
}
