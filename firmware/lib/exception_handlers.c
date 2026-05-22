#include "cpu.h"

#include "panic.h"

volatile Arm9ExceptionContext last_exception_context;

__attribute__((noreturn))
static void ExceptionRecordAndPanic(Arm9ExceptionKind kind, uint32_t fault_pc,
                                    uint32_t saved_cpsr)
{
    last_exception_context.kind = kind;
    last_exception_context.fault_pc = fault_pc;
    last_exception_context.saved_cpsr = saved_cpsr;

    if ((kind == ARM9_EXCEPTION_KIND_PREFETCH_ABORT) ||
        (kind == ARM9_EXCEPTION_KIND_DATA_ABORT)) {
        last_exception_context.fault_status = Arm9FaultStatusRead();
        last_exception_context.fault_address = Arm9FaultAddressRead();
    } else {
        last_exception_context.fault_status = 0;
        last_exception_context.fault_address = 0;
    }

    panic_exception(&last_exception_context);
}

__attribute__((weak))
void ExceptionHandleUndefinedInstruction(uint32_t fault_pc, uint32_t saved_cpsr)
{
    ExceptionRecordAndPanic(ARM9_EXCEPTION_KIND_UNDEFINED_INSTRUCTION, fault_pc, saved_cpsr);
}

__attribute__((weak))
void ExceptionHandleSoftwareInterrupt(uint32_t fault_pc, uint32_t saved_cpsr)
{
    ExceptionRecordAndPanic(ARM9_EXCEPTION_KIND_SOFTWARE_INTERRUPT, fault_pc, saved_cpsr);
}

__attribute__((weak))
void ExceptionHandlePrefetchAbort(uint32_t fault_pc, uint32_t saved_cpsr)
{
    ExceptionRecordAndPanic(ARM9_EXCEPTION_KIND_PREFETCH_ABORT, fault_pc, saved_cpsr);
}

__attribute__((weak))
void ExceptionHandleDataAbort(uint32_t fault_pc, uint32_t saved_cpsr)
{
    ExceptionRecordAndPanic(ARM9_EXCEPTION_KIND_DATA_ABORT, fault_pc, saved_cpsr);
}

__attribute__((weak))
void ExceptionHandleReservedVector(uint32_t fault_pc, uint32_t saved_cpsr)
{
    ExceptionRecordAndPanic(ARM9_EXCEPTION_KIND_RESERVED_VECTOR, fault_pc, saved_cpsr);
}

__attribute__((weak))
void ExceptionHandleIrq(uint32_t fault_pc, uint32_t saved_cpsr)
{
    ExceptionRecordAndPanic(ARM9_EXCEPTION_KIND_IRQ, fault_pc, saved_cpsr);
}

__attribute__((weak))
void ExceptionHandleFiq(uint32_t fault_pc, uint32_t saved_cpsr)
{
    ExceptionRecordAndPanic(ARM9_EXCEPTION_KIND_FIQ, fault_pc, saved_cpsr);
}
