#pragma once

#if defined(__WIIU__)
#include <nsysnet/socket.h>
#else
#include <sys/socket.h>
#endif

void network_init();

void network_exit();
