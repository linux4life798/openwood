#include <stdint.h>

#include "gpio.h"
#include "system.h"

#define SYSCFG0_PINMUX16 0x01c14160u
#define SYSCFG0_CFGCHIP0 0x01c1417cu
#define CFGCHIP0_PLL_MASTER_LOCK (1u << 4)

#define PLLC0_RSCTRL 0x01c110e8u
#define PLLC_RSCTRL_KEY_MASK 0x0000ffffu
#define PLLC_RSCTRL_KEY_UNLOCK 0x00005a69u
#define PLLC_RSCTRL_SWRST_RELEASED (1u << 16)

#define PINMUX16_GPIO7_12_MASK (0x0fu << 20)
#define PINMUX16_GPIO7_12_GPIO (0x08u << 20)

static const GPIOPin power_hold_gpio = {
    .bank = 7,
    .pin = 12,
};

static volatile uint32_t *MMIORegister32(uint32_t address)
{
    return (volatile uint32_t *)address;
}

static uint32_t MMIORead32(uint32_t address)
{
    return *MMIORegister32(address);
}

static void MMIOWrite32(uint32_t address, uint32_t value)
{
    *MMIORegister32(address) = value;
}

static void MMIOClearBits32(uint32_t address, uint32_t mask)
{
    MMIOWrite32(address, MMIORead32(address) & ~mask);
}

static void MMIOMaskedWrite32(uint32_t address, uint32_t mask, uint32_t value)
{
    MMIOWrite32(address, (MMIORead32(address) & ~mask) | (value & mask));
}

static void PinmuxSelectFunction(uint32_t pinmuxRegister, uint32_t fieldMask,
                                 uint32_t functionValue)
{
    MMIOMaskedWrite32(pinmuxRegister, fieldMask, functionValue);
}

static void PLLC0UnlockRegisters(void)
{
    /* CFGCHIP0.PLL_MASTER_LOCK must be clear before PLLC0 MMR writes stick. */
    MMIOClearBits32(SYSCFG0_CFGCHIP0, CFGCHIP0_PLL_MASTER_LOCK);
}

static void PLLC0UnlockResetControl(void)
{
    MMIOMaskedWrite32(PLLC0_RSCTRL, PLLC_RSCTRL_KEY_MASK, PLLC_RSCTRL_KEY_UNLOCK);
}

static void PLLC0AssertSoftwareReset(void)
{
    PLLC0UnlockRegisters();
    PLLC0UnlockResetControl();

    /* RSCTRL.SWRST is active-low: clearing it requests a software reset. */
    MMIOClearBits32(PLLC0_RSCTRL, PLLC_RSCTRL_SWRST_RELEASED);
}

static void PowerHoldRouteToGpio(void)
{
    PinmuxSelectFunction(SYSCFG0_PINMUX16, PINMUX16_GPIO7_12_MASK, PINMUX16_GPIO7_12_GPIO);
}

static void PowerHoldDriveLow(void)
{
    GPIOConfigureOutput(power_hold_gpio);
    GPIODriveLow(power_hold_gpio);
}

void SystemReboot(void)
{
    PLLC0AssertSoftwareReset();

    for (;;) {
    }
}

void SystemShutdown(void)
{
    PowerHoldRouteToGpio();

    /* 40CS low disables IC584, removes the 40C rail, and powers off the radio. */
    PowerHoldDriveLow();

    for (;;) {
    }
}
