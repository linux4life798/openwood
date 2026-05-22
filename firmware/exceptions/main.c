#include "cpu.h"
#include "rom_functions.h"
#include "rom_helpers.h"

int main(void)
{
    CPUInitialize();
    InitializePeripherals();

    DisplayRenderText("Trigger UNDF");
    TimerDelayMilliseconds(1000);

    TRAP_NO_RETURN();
}
