#include "gpio.h"
#include "utils.h"

#define GPIO_BASE 0x01e26000u

#define GPIO_PAIR_REGISTER_STRIDE 0x28u
#define GPIO_DIR_OFFSET 0x10u
#define GPIO_CLR_DATA_OFFSET 0x1cu
#define GPIO_IN_DATA_OFFSET 0x20u

const GPIOPin power_button_gpio = {
    .bank = 0,
    .pin = 1,
};

const GPIOPin ptt_button_gpio = {
    .bank = 0,
    .pin = 7,
};

static uint32_t GPIORegisterAddress(GPIOPin gpioPin, uint32_t offset)
{
    uint32_t bank_pair = gpioPin.bank / 2u;

    return GPIO_BASE + offset + (bank_pair * GPIO_PAIR_REGISTER_STRIDE);
}

static uint32_t GPIOPinMask(GPIOPin gpioPin)
{
    uint32_t bit_index = gpioPin.pin;

    if ((gpioPin.bank & 1u) != 0) {
        bit_index += 16u;
    }

    return (1u << bit_index);
}

void GPIOConfigureOutput(GPIOPin gpioPin)
{
    uint32_t direction_register = GPIORegisterAddress(gpioPin, GPIO_DIR_OFFSET);

    /* OMAP-L138 GPIO DIR uses 0 for output and 1 for input. */
    UtilsClearBits32(direction_register, GPIOPinMask(gpioPin));
}

void GPIODriveLow(GPIOPin gpioPin)
{
    UtilsWrite32(GPIORegisterAddress(gpioPin, GPIO_CLR_DATA_OFFSET),
                 GPIOPinMask(gpioPin));
}

bool GPIOReadPin(GPIOPin gpioPin)
{
    return ((UtilsRead32(GPIORegisterAddress(gpioPin, GPIO_IN_DATA_OFFSET)) &
             GPIOPinMask(gpioPin)) != 0);
}
