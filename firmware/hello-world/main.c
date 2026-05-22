#include "cpu.h"
#include "rom_functions.h"
#include "rom_helpers.h"

/*
 * Initialize the ROM peripherals, display "Hello World!",
 * then idle forever.
 */
int main(void)
{
    CPUInitialize();
    InitializePeripherals();

    DisplayRenderText("Hello World!");

    for (;;)
        ;
}
