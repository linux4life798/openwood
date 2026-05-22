#include "cpu.h"

.section .text.startup, "ax"
.arm

@ Switch to an exception CPU mode, assign its banked stack pointer, then reserve
@ a fixed-size stack slot below it for the next mode.
.macro set_mode_stack mode
    msr cpsr_c, #(\mode | ARM9_CPSR_INTERRUPT_DISABLE)
    mov sp, r0
    sub r0, r0, #ARM9_EXCEPTION_STACK_SIZE
.endm

.global ResetHandler
.global _start
ResetHandler:
_start:
    @ ARM exception modes have banked SP registers. Initialize each one before
    @ C code can run so an early exception has a valid stack to use.
    ldr r0, =__stack_top
    set_mode_stack ARM9_CPSR_MODE_UNDEFINED
    set_mode_stack ARM9_CPSR_MODE_ABORT
    set_mode_stack ARM9_CPSR_MODE_FIQ
    set_mode_stack ARM9_CPSR_MODE_IRQ
    set_mode_stack ARM9_CPSR_MODE_SUPERVISOR

    @ Run normal firmware code in System mode using the remaining stack space.
    @ Keep IRQ/FIQ masked until firmware explicitly enables them.
    msr cpsr_c, #(ARM9_CPSR_MODE_SYSTEM | ARM9_CPSR_INTERRUPT_DISABLE)
    mov sp, r0

    @ Zero .bss before entering C.
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0

clear_bss:
    cmp r0, r1
    bhs bss_done
    str r2, [r0], #4
    b clear_bss

bss_done:
    @ Low vectors are already present at the image base. This can be enabled
    @ later if we want the ARM9 high-vector address at 0xffff0000 instead.
    @ bl Arm9HighVectorsConfigureNoMMU
    bl main

hang:
    @ main should not return.
    b hang
