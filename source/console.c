#if defined(__WIIU__)

#include <whb/log.h>
#include <whb/log_console.h>
#include <stdio.h>
#include <stdarg.h>

void console_init()
{
	WHBLogConsoleInit();
}

void console_exit()
{
	WHBLogConsoleFree();
}

void console_write(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	char *buffer = new char[2048];
	vsprintf(buffer, format, args);
	WHBLogWrite(buffer);
	WHBLogConsoleDraw();
	delete buffer;
	va_end(args);
}

#elif defined(__SWITCH__)

#include <switch.h>
#include <stdio.h>
#include <stdarg.h>

void console_init()
{
	consoleInit(NULL);
}

void console_exit()
{
	consoleExit(NULL);
}

void console_write(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	consoleUpdate(NULL);
	va_end(args);
}

#else

#include <stdio.h>
#include <stdarg.h>

void console_init() {}

void console_exit() {}

void console_write(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

#endif
