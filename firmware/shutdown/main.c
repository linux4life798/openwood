#include "cpu.h"
#include "system.h"
#include "rom_functions.h"
#include "rom_helpers.h"

/*
 * Show a prompt, then display the shutdown message when the Power button is
 * pressed.
 */
int main(void)
{
    CPUInitialize();
    InitializePeripherals();

    DisplayRenderText("Press Power");
    while (!ButtonCheckPowerPressed()) {
        TimerDelayMilliseconds(50);
    }

    DisplayRenderText("Shutdown");
    TimerDelayMilliseconds(500);

    SystemShutdown();
}
