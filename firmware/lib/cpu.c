#include "cpu.h"

extern const Arm9VectorTable __vector_table;

void CPUInitialize(void)
{
    /* Setup the exception vectors and disable the MMU and caches. */
    Arm9HighVectorsConfigureNoMMU();
}


uint32_t Arm9ControlRegisterRead(void)
{
    uint32_t value;

    asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(value)::"memory");

    return value;
}

void Arm9ControlRegisterWrite(uint32_t value)
{
    asm volatile("mcr p15, 0, %0, c1, c0, 0" ::"r"(value) : "memory");
}

uint32_t Arm9TranslationTableBaseRead(void)
{
    uint32_t value;

    asm volatile("mrc p15, 0, %0, c2, c0, 0" : "=r"(value)::"memory");

    return value;
}

void Arm9TranslationTableBaseWrite(uint32_t value)
{
    asm volatile("mcr p15, 0, %0, c2, c0, 0" ::"r"(value) : "memory");
}

uint32_t Arm9DomainAccessControlRead(void)
{
    uint32_t value;

    asm volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(value)::"memory");

    return value;
}

void Arm9DomainAccessControlWrite(uint32_t value)
{
    asm volatile("mcr p15, 0, %0, c3, c0, 0" ::"r"(value) : "memory");
}

uint32_t Arm9FaultStatusRead(void)
{
    uint32_t value;

    asm volatile("mrc p15, 0, %0, c5, c0, 0" : "=r"(value)::"memory");

    return value;
}

uint32_t Arm9FaultAddressRead(void)
{
    uint32_t value;

    asm volatile("mrc p15, 0, %0, c6, c0, 0" : "=r"(value)::"memory");

    return value;
}

void Arm9FCSEPIDWrite(uint32_t value)
{
    asm volatile("mcr p15, 0, %0, c13, c0, 0" ::"r"(value) : "memory");
}

bool Arm9MMUIsEnabled(uint32_t control)
{
    return ((control & ARM9_CP15_CONTROL_MMU_ENABLE) != 0);
}

void Arm9MMUDisable(void)
{
    uint32_t control = Arm9ControlRegisterRead();

    control &= ~ARM9_CP15_CONTROL_MMU_ENABLE;
    Arm9ControlRegisterWrite(control);
}

Arm9ProgramStatus Arm9SavedProgramStatusRead(void)
{
    Arm9ProgramStatus status;

    asm volatile("mrs %0, spsr" : "=r"(status.raw));

    return status;
}

static void Arm9HighVectorTableCopy(void)
{
    volatile Arm9VectorTable *vector_table =
        (volatile Arm9VectorTable *)(void *)ARM9_HIGH_VECTOR_BASE;

    *vector_table = __vector_table;
}

void Arm9HighVectorsConfigureNoMMU(void)
{
    uint32_t control;

    Arm9HighVectorTableCopy();

    control = Arm9ControlRegisterRead();
    control &= ~(ARM9_CP15_CONTROL_MMU_ENABLE |
                 ARM9_CP15_CONTROL_DATA_CACHE_ENABLE |
                 ARM9_CP15_CONTROL_INSTRUCTION_CACHE_ENABLE);
    control |= ARM9_CP15_CONTROL_HIGH_EXCEPTION_VECTORS;
    Arm9ControlRegisterWrite(control);
}
