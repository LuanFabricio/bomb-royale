#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../types.h"
#include "../utils/net.h"
#include "../levels/level.h"

#define SERVER_PORT 4242

int main()
{
	Block blocks[GRID_LENGTH * GRID_LENGTH] = {0};
	size_t blocks_size = size;

	for (size_t i = 0; i < blocks_size; i++) {
		blocks[i] = ((Block*)level_bytes)[i];
	}

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd < 0) {
		fprintf(stderr, "[Error] Server not initialized.");
		exit(EXIT_FAILURE);
	}

	printf("Socket fd: %i\n", server_fd);

	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_addr = {
			.s_addr = INADDR_ANY,
		},
		.sin_port = htons(SERVER_PORT),
	};
	size_t socket_len = sizeof(server_addr);


	if (bind(server_fd, (const struct sockaddr *)&server_addr, socket_len) < 0) {
		fprintf(stderr, "[Error] On socket binding: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		fprintf(stderr, "[Error] %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	while (1) {
		struct sockaddr_in client_addr = {0};
		socklen_t client_len = sizeof(client_addr);

		int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

		ServerData data = {0};
		ssize_t bytes = recv(client_fd, &data, sizeof(ServerData), 0);

		printf("[");
		Net_print_event(data.event);
		printf("]");

		switch (data.event) {
		case PING:
			printf("Pong");
			break;
		case REQUEST_MAP:
			data.event = UPDATE_MAP;
			memcpy(data.GameMap.data, blocks, blocks_size * sizeof(Block));
			data.GameMap.size = blocks_size;
			break;

		case REQUEST_PLAYERS:

			break;
		default:
			break;
		}

		send(client_fd, &data, sizeof(ServerData), 0);
		close(client_fd);
		if (blocks_size > 0) blocks_size--;
	}

	close(server_fd);

	return 0;
}
