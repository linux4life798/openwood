#include "cpu.h"
#include "rom_functions.h"
#include "rom_helpers.h"

/*
 * Display a warning, trigger an undefined instruction, and
 * let the default exception handler render the panic information.
 */
int main(void)
{
    CPUInitialize();
    InitializePeripherals();

    DisplayRenderText("Trigger UNDF");
    TimerDelayMilliseconds(1000);

    TRAP_NO_RETURN();
}
