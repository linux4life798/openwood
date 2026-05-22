#include <stdint.h>

#include "rom_functions.h"
#include "rom_helpers.h"

int main(void)
{
    InitializePeripherals();
    Arm9HighVectorsConfigureNoMMU();
    DisplayRenderText("Hello World!");

    for (;;)
        ;
}
