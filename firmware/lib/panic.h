#ifndef PANIC_H
#define PANIC_H

#include "cpu.h"

__attribute__((noreturn))
void panic(const char *msg);

__attribute__((noreturn))
void panic_exception(const volatile Arm9ExceptionContext *context);

#endif // PANIC_H
