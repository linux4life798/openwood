#include <stdint.h>

#include "cpu.h"
#include "rom_functions.h"
#include "rom_helpers.h"

int main(void)
{
    CPUInitialize();
    InitializePeripherals();

    DisplayRenderText("Hello World!");

    for (;;)
        ;
}
