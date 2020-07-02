#pragma once
#include <stdarg.h>

void console_init();
void console_exit();
void console_write(const char *format, ...);
