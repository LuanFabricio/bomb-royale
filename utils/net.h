#ifndef NET_H_
#define NET_H_

#include "../types.h"

#ifndef PLATFORM_WEB
#include <stdio.h>
static void Net_print_event(GameNetEvent event)
{
	switch (event) {
	case PING:
		printf("Ping");
		break;
	case UPDATE_MAP:
		printf("Update Map");
		break;
	case UPDATE_PLAYERS:
		printf("Update Players");
		break;
	case REQUEST_MAP:
		printf("Request Map");
		break;
	case REQUEST_PLAYERS:
		printf("Request Players");
		break;
	}
}
#endif

#endif // NET_H_
