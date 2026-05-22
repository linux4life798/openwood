#include <stddef.h>

#include "panic.h"

#include "rom_functions.h"
#include "utils.h"

static const char *panic_exception_label(Arm9ExceptionKind kind)
{
    switch (kind) {
    case ARM9_EXCEPTION_KIND_UNDEFINED_INSTRUCTION:
        return "UNDF";
    case ARM9_EXCEPTION_KIND_SOFTWARE_INTERRUPT:
        return "SWI ";
    case ARM9_EXCEPTION_KIND_PREFETCH_ABORT:
        return "PABT";
    case ARM9_EXCEPTION_KIND_DATA_ABORT:
        return "DABT";
    case ARM9_EXCEPTION_KIND_RESERVED_VECTOR:
        return "RSRV";
    case ARM9_EXCEPTION_KIND_IRQ:
        return "IRQ ";
    case ARM9_EXCEPTION_KIND_FIQ:
        return "FIQ ";
    default:
        return "EXC?";
    }
}

void panic(const char *msg)
{
    char fallback_msg[] = "PANIC";

    if (msg == NULL) {
        msg = fallback_msg;
    }

    PanicDisplayRenderText((char *)msg);

    for (;;) {
    }
}

void panic_exception(const volatile Arm9ExceptionContext *context)
{
    char line[] = "EXC?:--------";
    const char *label;

    if (context == NULL) {
        panic(NULL);
    }

    label = panic_exception_label(context->kind);
    line[0] = label[0];
    line[1] = label[1];
    line[2] = label[2];
    line[3] = label[3];
    UtilsFormatHexWord(&line[5], context->fault_pc);

    panic(line);
}
