#include <stdint.h>

#include "cpu.h"
#include "rom_functions.h"
#include "rom_helpers.h"

__attribute__((noreturn))
static void trigger_undefined_instruction(void)
{
    __asm__ volatile("udf #0" ::: "memory");

    for (;;)
        ;
}

int main(void)
{
    InitializePeripherals();

    DisplayRenderText("Exception test");
    TimerDelayMilliseconds(1000);

    Arm9HighVectorsConfigureNoMMU();

    DisplayRenderText("Trigger UNDF");
    TimerDelayMilliseconds(1000);

    trigger_undefined_instruction();

    for (;;)
        ;
}
