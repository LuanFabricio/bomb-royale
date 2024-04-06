#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../defines.h"
#include "../types.h"
#include "../utils/net.h"
#include "../quadtree.h"
#include "../platform.h"

#define SERVER_PORT 4242

static int client_fd = 0;
static struct sockaddr_in server_addr = {0};
static size_t socket_len = 0;

static QuadTree *root = NULL;

void init_client_server()
{
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0) {
		fprintf(stderr, "[Error] Server not initialized.");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVER_PORT);

	socket_len = sizeof(server_addr);
}

void game_loop()
{
	Platform_begin_drawing();

	Platform_clear_background(0xfffbfbfb);
	QuadTree_display(root);

	Platform_end_drawing();
}

int main()
{
	ServerData data = { 0 };

	init_client_server();
	int status = connect(client_fd, (const struct sockaddr *)&server_addr, socket_len);

	data.event = REQUEST_MAP;

	send(client_fd, &data, sizeof(ServerData), 0);
	recv(client_fd, &data, sizeof(ServerData), 0);

	Block grid[GRID_LENGTH * GRID_LENGTH] = {0};
	memcpy(&grid, data.GameMap.data, sizeof(Block) * data.GameMap.size);

	root = QuadTree_new((float)HALF_WIDTH, (float)HALF_HEIGHT, (float)HALF_WIDTH);
	for (u64 i = 0; i < data.GameMap.size ; i++) {
		// TODO: Fix this hack
		// NOTE: level editor saves with 0 index
		// and the game uses 1 index
		grid[i].center.x += GRID_SIZE;
		grid[i].center.y += GRID_SIZE;
		QuadTree_insert(root, grid[i]);
	}

	printf("[");
	Net_print_event(data.event);
	printf("](%u)\n", data.GameMap.size);
	for (size_t i = 0; i < data.GameMap.size; i++) {
		printf("Block[%lu]:\n", i);
		printf("\tuid: %u\n", data.GameMap.data[i].uid);
		printf("\tType: %u\n", data.GameMap.data[i].grid_type);
		printf("\tPosition: %.2f, %.2f\n",
				data.GameMap.data[i].center.x,
				data.GameMap.data[i].center.y);
	}

	Platform_init_window("Client", SCREEN_WIDTH, SCREEN_HEIGHT);

	while(!Platform_window_should_close()) {
		game_loop();
	}

	Platform_close_window();
	close(client_fd);

	return 0;
}
