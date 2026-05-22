#include "cpu.h"

#include "panic.h"

#define ARM9_INTERRUPT(kind) __attribute__((interrupt(kind)))
#define EXCEPTION_FAULT_PC(offset) ((uint32_t)(uintptr_t)__builtin_return_address(0) - (offset))

volatile Arm9ExceptionContext last_exception_context;

/*
 * These vector targets use GCC interrupt attributes so a custom weak-handler
 * override can return through GCC's generated exception epilogue.
 */
static volatile Arm9ExceptionContext *ExceptionRecord(Arm9ExceptionKind kind, uint32_t fault_pc)
{
    last_exception_context = (Arm9ExceptionContext){ 0 };

    last_exception_context.kind = kind;
    last_exception_context.fault_pc = fault_pc;
    last_exception_context.saved_cpsr = Arm9SavedProgramStatusRead();

    if ((kind == ARM9_EXCEPTION_KIND_PREFETCH_ABORT) ||
        (kind == ARM9_EXCEPTION_KIND_DATA_ABORT)) {
        last_exception_context.fault_status = Arm9FaultStatusRead();
        last_exception_context.fault_address = Arm9FaultAddressRead();
    }

    return &last_exception_context;
}

void UndefinedInstructionHandler(void) ARM9_INTERRUPT("UNDEF");
void UndefinedInstructionHandler(void)
{
    ExceptionHandleUndefinedInstruction(
        ExceptionRecord(ARM9_EXCEPTION_KIND_UNDEFINED_INSTRUCTION,
                        EXCEPTION_FAULT_PC(ARM9_EXCEPTION_RETURN_OFFSET_UNDEFINED)));
}

void SoftwareInterruptHandler(void) ARM9_INTERRUPT("SWI");
void SoftwareInterruptHandler(void)
{
    ExceptionHandleSoftwareInterrupt(
        ExceptionRecord(ARM9_EXCEPTION_KIND_SOFTWARE_INTERRUPT,
                        EXCEPTION_FAULT_PC(ARM9_EXCEPTION_RETURN_OFFSET_SWI)));
}

void PrefetchAbortHandler(void) ARM9_INTERRUPT("ABORT");
void PrefetchAbortHandler(void)
{
    ExceptionHandlePrefetchAbort(
        ExceptionRecord(ARM9_EXCEPTION_KIND_PREFETCH_ABORT,
                        EXCEPTION_FAULT_PC(ARM9_EXCEPTION_RETURN_OFFSET_PREFETCH_ABORT)));
}

void DataAbortHandler(void) ARM9_INTERRUPT("ABORT");
void DataAbortHandler(void)
{
    ExceptionHandleDataAbort(
        ExceptionRecord(ARM9_EXCEPTION_KIND_DATA_ABORT,
                        EXCEPTION_FAULT_PC(ARM9_EXCEPTION_RETURN_OFFSET_DATA_ABORT)));
}

void ReservedVectorHandler(void) ARM9_INTERRUPT("UNDEF");
void ReservedVectorHandler(void)
{
    ExceptionHandleReservedVector(
        ExceptionRecord(ARM9_EXCEPTION_KIND_RESERVED_VECTOR,
                        EXCEPTION_FAULT_PC(ARM9_EXCEPTION_RETURN_OFFSET_RESERVED)));
}

void IrqHandler(void) ARM9_INTERRUPT("IRQ");
void IrqHandler(void)
{
    ExceptionHandleIrq(
        ExceptionRecord(ARM9_EXCEPTION_KIND_IRQ,
                        EXCEPTION_FAULT_PC(ARM9_EXCEPTION_RETURN_OFFSET_IRQ)));
}

void FiqHandler(void) ARM9_INTERRUPT("FIQ");
void FiqHandler(void)
{
    ExceptionHandleFiq(
        ExceptionRecord(ARM9_EXCEPTION_KIND_FIQ,
                        EXCEPTION_FAULT_PC(ARM9_EXCEPTION_RETURN_OFFSET_FIQ)));
}

__attribute__((weak))
void ExceptionHandleUndefinedInstruction(volatile Arm9ExceptionContext *context)
{
    panic_exception(context);
}

__attribute__((weak))
void ExceptionHandleSoftwareInterrupt(volatile Arm9ExceptionContext *context)
{
    panic_exception(context);
}

__attribute__((weak))
void ExceptionHandlePrefetchAbort(volatile Arm9ExceptionContext *context)
{
    panic_exception(context);
}

__attribute__((weak))
void ExceptionHandleDataAbort(volatile Arm9ExceptionContext *context)
{
    panic_exception(context);
}

__attribute__((weak))
void ExceptionHandleReservedVector(volatile Arm9ExceptionContext *context)
{
    panic_exception(context);
}

__attribute__((weak))
void ExceptionHandleIrq(volatile Arm9ExceptionContext *context)
{
    panic_exception(context);
}

__attribute__((weak))
void ExceptionHandleFiq(volatile Arm9ExceptionContext *context)
{
    panic_exception(context);
}
