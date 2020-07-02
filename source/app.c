#if defined(__WIIU__)

#include <whb/proc.h>
#include <stdbool.h>

void app_init()
{
	WHBProcInit();
}

void app_exit()
{
	WHBProcShutdown();
}

bool app_running()
{
	return (WHBProcIsRunning() == TRUE);
}

#elif defined(__SWITCH__)

#include <switch.h>
#include <stdbool.h>

void app_init() {}

void app_exit() {}

bool app_running()
{
	hidScanInput();
	return (
		!(hidKeysDown(CONTROLLER_P1_AUTO) & KEY_PLUS) &&
		appletMainLoop()
	);
}

#else

#include <stdbool.h>

void app_init() {}

void app_exit() {}

bool app_running()
{
	return false;
}

#endif

