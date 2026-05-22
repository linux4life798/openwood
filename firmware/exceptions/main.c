#include "cpu.h"
#include "rom_functions.h"
#include "rom_helpers.h"

int main(void)
{
    InitializePeripherals();
    Arm9HighVectorsConfigureNoMMU();

    DisplayRenderText("Trigger UNDF");
    TimerDelayMilliseconds(1000);

    TRAP_NO_RETURN();
}
