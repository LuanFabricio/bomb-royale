#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../types.h"
#include "../utils/net.h"

#define SERVER_PORT 4242

int main()
{
	printf("Hello world\n");
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (client_fd < 0) {
		fprintf(stderr, "[Error] Server not initialized.");
		exit(EXIT_FAILURE);
	}

	printf("Socket fd: %i\n", client_fd);

	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_addr = {
			.s_addr = INADDR_ANY,
		},
		.sin_port = htons(SERVER_PORT),
	};
	size_t socket_len = sizeof(server_addr);

	int status = connect(client_fd, (const struct sockaddr *)&server_addr, socket_len);

	ServerData data = {
		.event 	= REQUEST_MAP,
		0
	};

	send(client_fd, &data, sizeof(ServerData), 0);

	recv(client_fd, &data, sizeof(ServerData), 0);

	printf("[");
	Net_print_event(data.event);
	printf("]\n");
	for (size_t i = 0; i < data.GameMap.size; i++) {
		printf("Block[%lu]:\n", i);
		printf("\tuid: %u\n", data.GameMap.data[i].uid);
		printf("\tType: %u\n", data.GameMap.data[i].grid_type);
		printf("\tPosition: %.2f, %.2f\n",
				data.GameMap.data[i].center.x,
				data.GameMap.data[i].center.y);
	}

	close(client_fd);

	return 0;
}
