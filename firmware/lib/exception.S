#include "cpu.h"

.section .text.exception, "ax"
.arm

@ Convert hardware exception entry into a normal C call:
@ r0 = fault/interrupted PC, adjusted for the exception type
@ r1 = saved CPSR from before the exception
@ The C handler is expected not to return. If it does, stop here instead of
@ attempting an exception return with an unknown restored context.
.macro dispatch_exception c_handler, pc_offset
    sub r0, lr, #\pc_offset
    mrs r1, spsr

    @ Ensure the C handler runs with both interrupt classes masked.
    mrs r2, cpsr
    orr r2, r2, #ARM9_CPSR_INTERRUPT_DISABLE
    msr cpsr_c, r2

    bl \c_handler
    b unhandled_exception
.endm

@ Each vector lands in the CPU mode selected by hardware for that exception.
@ These veneers preserve the mode stack and only pass minimal fault context to C.
.global UndefinedInstructionHandler
.type UndefinedInstructionHandler, %function
UndefinedInstructionHandler:
    dispatch_exception ExceptionHandleUndefinedInstruction, ARM9_EXCEPTION_PC_OFFSET_UNDEFINED
.size UndefinedInstructionHandler, . - UndefinedInstructionHandler

.global SoftwareInterruptHandler
.type SoftwareInterruptHandler, %function
SoftwareInterruptHandler:
    dispatch_exception ExceptionHandleSoftwareInterrupt, ARM9_EXCEPTION_PC_OFFSET_SWI
.size SoftwareInterruptHandler, . - SoftwareInterruptHandler

.global PrefetchAbortHandler
.type PrefetchAbortHandler, %function
PrefetchAbortHandler:
    dispatch_exception ExceptionHandlePrefetchAbort, ARM9_EXCEPTION_PC_OFFSET_PREFETCH_ABORT
.size PrefetchAbortHandler, . - PrefetchAbortHandler

.global DataAbortHandler
.type DataAbortHandler, %function
DataAbortHandler:
    dispatch_exception ExceptionHandleDataAbort, ARM9_EXCEPTION_PC_OFFSET_DATA_ABORT
.size DataAbortHandler, . - DataAbortHandler

.global ReservedVectorHandler
.type ReservedVectorHandler, %function
ReservedVectorHandler:
    dispatch_exception ExceptionHandleReservedVector, ARM9_EXCEPTION_PC_OFFSET_RESERVED
.size ReservedVectorHandler, . - ReservedVectorHandler

.global IrqHandler
.type IrqHandler, %function
IrqHandler:
    dispatch_exception ExceptionHandleIrq, ARM9_EXCEPTION_PC_OFFSET_IRQ
.size IrqHandler, . - IrqHandler

.global FiqHandler
.type FiqHandler, %function
FiqHandler:
    dispatch_exception ExceptionHandleFiq, ARM9_EXCEPTION_PC_OFFSET_FIQ
.size FiqHandler, . - FiqHandler

unhandled_exception:
    @ Preserve the stopped state for debugger inspection.
    b unhandled_exception
