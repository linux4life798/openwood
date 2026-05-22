#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t *)(addr))

void SystemReboot(void)
{
    REG32(0x01c1417c) &= ~0x10u;                         // unlock PLLC0 MMRs via CFGCHIP0
    REG32(0x01c110e8) = (REG32(0x01c110e8) & 0xffff0000u) | 0x5a69u;
    REG32(0x01c110e8) &= ~0x00010000u;                   // RSCTRL.SWRST = 0
}

void SystemShutdown(void)
{
    /* Route the 40CS pin as GPIO7_12. */
    REG32(0x01c14160) = (REG32(0x01c14160) & ~0x00f00000u) | 0x00800000u;

    /* GPIO DIR bit: 0 = output on OMAP-L138. */
    REG32(0x01e26088) &= ~0x10000000u;

    /* Drive 40CS low; this disables IC584 and kills 40C. */
    REG32(0x01e26094) = 0x10000000u;

    for (;;) {
    }
}
