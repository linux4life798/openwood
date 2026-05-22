#ifndef CPU_H
#define CPU_H

#define ARM9_CP15_CONTROL_MMU_ENABLE                 (1u << 0)
#define ARM9_CP15_CONTROL_ALIGNMENT_FAULT_ENABLE     (1u << 1)
#define ARM9_CP15_CONTROL_DATA_CACHE_ENABLE          (1u << 2)
#define ARM9_CP15_CONTROL_BIG_ENDIAN_ENABLE          (1u << 7)
#define ARM9_CP15_CONTROL_SYSTEM_PROTECTION_ENABLE   (1u << 8)
#define ARM9_CP15_CONTROL_ROM_PROTECTION_ENABLE      (1u << 9)
#define ARM9_CP15_CONTROL_INSTRUCTION_CACHE_ENABLE   (1u << 12)
#define ARM9_CP15_CONTROL_HIGH_EXCEPTION_VECTORS     (1u << 13)
#define ARM9_CP15_CONTROL_ROUND_ROBIN_CACHE_ENABLE   (1u << 14)

#define ARM9_CPSR_MODE_USER                          0x10
#define ARM9_CPSR_MODE_FIQ                           0x11
#define ARM9_CPSR_MODE_IRQ                           0x12
#define ARM9_CPSR_MODE_SUPERVISOR                    0x13
#define ARM9_CPSR_MODE_ABORT                         0x17
#define ARM9_CPSR_MODE_UNDEFINED                     0x1b
#define ARM9_CPSR_MODE_SYSTEM                        0x1f
#define ARM9_CPSR_FIQ_DISABLE                        (1 << 6)
#define ARM9_CPSR_IRQ_DISABLE                        (1 << 7)
#define ARM9_CPSR_INTERRUPT_DISABLE                  (ARM9_CPSR_FIQ_DISABLE | ARM9_CPSR_IRQ_DISABLE)

#define ARM9_HIGH_VECTOR_BASE                        0xffff0000u
#define ARM9_VECTOR_COUNT                            8u
#define ARM9_VECTOR_TABLE_SIZE                       0x40
#define ARM9_EXCEPTION_STACK_SIZE                    1024

/*
 * Offsets from GCC's interrupt-wrapper return address to the instruction that
 * caused or was interrupted by the exception. GCC pre-adjusts LR for IRQ, FIQ,
 * and ABORT handlers before C code runs.
 */
#define ARM9_EXCEPTION_RETURN_OFFSET_UNDEFINED       4u
#define ARM9_EXCEPTION_RETURN_OFFSET_SWI             4u
#define ARM9_EXCEPTION_RETURN_OFFSET_PREFETCH_ABORT  0u
#define ARM9_EXCEPTION_RETURN_OFFSET_DATA_ABORT      4u
#define ARM9_EXCEPTION_RETURN_OFFSET_RESERVED        0u
#define ARM9_EXCEPTION_RETURN_OFFSET_IRQ             0u
#define ARM9_EXCEPTION_RETURN_OFFSET_FIQ             0u

#ifndef __ASSEMBLER__

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t instructions[ARM9_VECTOR_COUNT];
    uint32_t handler_addresses[ARM9_VECTOR_COUNT];
} Arm9VectorTable;

typedef enum {
    ARM9_EXCEPTION_KIND_UNDEFINED_INSTRUCTION,
    ARM9_EXCEPTION_KIND_SOFTWARE_INTERRUPT,
    ARM9_EXCEPTION_KIND_PREFETCH_ABORT,
    ARM9_EXCEPTION_KIND_DATA_ABORT,
    ARM9_EXCEPTION_KIND_RESERVED_VECTOR,
    ARM9_EXCEPTION_KIND_IRQ,
    ARM9_EXCEPTION_KIND_FIQ,
} Arm9ExceptionKind;

typedef enum {
    ARM9_PROGRAM_MODE_USER = ARM9_CPSR_MODE_USER,
    ARM9_PROGRAM_MODE_FIQ = ARM9_CPSR_MODE_FIQ,
    ARM9_PROGRAM_MODE_IRQ = ARM9_CPSR_MODE_IRQ,
    ARM9_PROGRAM_MODE_SUPERVISOR = ARM9_CPSR_MODE_SUPERVISOR,
    ARM9_PROGRAM_MODE_ABORT = ARM9_CPSR_MODE_ABORT,
    ARM9_PROGRAM_MODE_UNDEFINED = ARM9_CPSR_MODE_UNDEFINED,
    ARM9_PROGRAM_MODE_SYSTEM = ARM9_CPSR_MODE_SYSTEM,
} Arm9ProgramMode;

typedef union {
    uint32_t raw;
    struct {
        Arm9ProgramMode mode : 5;
        uint32_t thumb_state : 1;
        uint32_t fiq_disabled : 1;
        uint32_t irq_disabled : 1;
        uint32_t reserved0 : 16;
        uint32_t jazelle_state : 1;
        uint32_t reserved1 : 2;
        uint32_t cumulative_saturation : 1;
        uint32_t overflow : 1;
        uint32_t carry : 1;
        uint32_t zero : 1;
        uint32_t negative : 1;
    } bits;
} Arm9ProgramStatus;

typedef struct {
    Arm9ExceptionKind kind;
    uint32_t fault_pc;
    Arm9ProgramStatus saved_cpsr;
    uint32_t fault_status;
    uint32_t fault_address;
} Arm9ExceptionContext;

_Static_assert(sizeof(Arm9VectorTable) == ARM9_VECTOR_TABLE_SIZE,
               "ARM9 vector table size mismatch");
_Static_assert(sizeof(Arm9ProgramStatus) == sizeof(uint32_t),
               "ARM9 program status size mismatch");

/**
 * Default handlers record last_exception_context and panic. Applications can
 * override any handler by defining the same symbol.
 */
void ExceptionHandleUndefinedInstruction(volatile Arm9ExceptionContext *context);
void ExceptionHandleSoftwareInterrupt(volatile Arm9ExceptionContext *context);
void ExceptionHandlePrefetchAbort(volatile Arm9ExceptionContext *context);
void ExceptionHandleDataAbort(volatile Arm9ExceptionContext *context);
void ExceptionHandleReservedVector(volatile Arm9ExceptionContext *context);
void ExceptionHandleIrq(volatile Arm9ExceptionContext *context);
void ExceptionHandleFiq(volatile Arm9ExceptionContext *context);

/* The compiler builtin trap will remove "unreachable" code after this call. */
#define TRAP_NO_RETURN() __builtin_trap()
/* The asm volatile will not remove code found after the call. */
#define TRAP_YES_RETURN() asm volatile("udf #0" ::: "memory")

/**
 * Reads the current exception mode's saved program status register.
 */
Arm9ProgramStatus Arm9SavedProgramStatusRead(void);

/**
 * Reads the ARM9 CP15 control register (c1).
 */
uint32_t Arm9ControlRegisterRead(void);

/**
 * Writes the ARM9 CP15 control register (c1).
 */
void Arm9ControlRegisterWrite(uint32_t value);

/**
 * Reads the ARM9 CP15 translation table base register (c2).
 */
uint32_t Arm9TranslationTableBaseRead(void);

/**
 * Writes the ARM9 CP15 translation table base register (c2).
 */
void Arm9TranslationTableBaseWrite(uint32_t value);

/**
 * Reads the ARM9 CP15 domain access control register (c3).
 */
uint32_t Arm9DomainAccessControlRead(void);

/**
 * Writes the ARM9 CP15 domain access control register (c3).
 */
void Arm9DomainAccessControlWrite(uint32_t value);

/**
 * Reads the ARM9 CP15 fault status register (c5).
 */
uint32_t Arm9FaultStatusRead(void);

/**
 * Reads the ARM9 CP15 fault address register (c6).
 */
uint32_t Arm9FaultAddressRead(void);

/**
 * Writes the ARM9 CP15 FCSE PID register (c13).
 */
void Arm9FCSEPIDWrite(uint32_t value);

/**
 * Returns true when the CP15 control register has the MMU enable bit set.
 */
bool Arm9MMUIsEnabled(uint32_t control);

/**
 * Disables address translation by clearing the CP15 control MMU enable bit.
 */
void Arm9MMUDisable(void);

/**
 * Disables the MMU/caches and selects the copied vector table at 0xffff0000.
 */
void Arm9HighVectorsConfigureNoMMU(void);

#endif

#endif // CPU_H
