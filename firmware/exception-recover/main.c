#include <stdint.h>

#include "cpu.h"
#include "rom_functions.h"
#include "rom_helpers.h"

/* Register an handler for undefined instructions that will continue */
void ExceptionHandleUndefinedInstruction(volatile Arm9ExceptionContext *)
{
    DisplayRenderText("In handler");
    TimerDelayMilliseconds(1000);
}

/*
 * Trigger an undefined instruction, show the custom handler
 * message, return from the handler, then display "After trap" and idle.
 */
int main(void)
{
    CPUInitialize();
    InitializePeripherals();

    DisplayRenderText("Trigger UNDF");
    TimerDelayMilliseconds(1000);

    TRAP_YES_RETURN();

    DisplayRenderText("After trap");

    for (;;)
        ;
}
