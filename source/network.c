#if defined(__WIIU__)

#include <nsysnet/socket.h>
#include <nsysnet/nssl.h>
#include <nn/ac.h>

void network_init()
{
	ACConfigId configId;
	ACInitialize();
	ACGetStartupId(&configId);
	ACConnectWithConfigId(configId);
	socket_lib_init();
}

void network_exit()
{
	socket_lib_finish();
	ACFinalize();
}

#elif defined(__SWITCH__)

#include <switch.h>

void network_init()
{
	socketInitializeDefault();
}

void network_exit()
{
	socketExit();
}

#else

void network_init() {}

void network_exit() {}

#endif
