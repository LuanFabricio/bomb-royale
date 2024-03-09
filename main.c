#include "defines.h"
#include "mm.h"
#include "quadtree.h"
#include "./platform.h"
#include "types.h"
#include "levels/level.h"
#include <stdio.h>

int main()
{
	QuadTree *root = QuadTree_new((float)HALF_WIDTH, (float)HALF_HEIGHT, (float)HALF_WIDTH);

	u32 blocks_size = 0;
	Block *grid = (Block*)level_bytes;

	for (u64 i = 0; i < size ; i++) {
		QuadTree_insert(root, grid[i]);
	}

	Player players[] = {
		{ .id = 1, .center = (Point){ GRID_SIZE*4 , GRID_SIZE*4  }, },
		{ .id = 2, .center = (Point){ GRID_SIZE*1 , GRID_SIZE*4  }, },
		{ .id = 3, .center = (Point){ GRID_SIZE*13, GRID_SIZE*13 }, },
	};
	u8 my_id = 1;
	u8 my_id_idx = 0;

	u8 players_len = sizeof(players)/sizeof(players[0]);
	for (u8 i = 0; i < players_len; i++) {
		if (players[i].id == my_id_idx) {
			my_id_idx = i;
			break;
		}
	}

	mm_log();

	Platform_init_window("Bomb royale", SCREEN_WIDTH, SCREEN_HEIGHT);

	while (!Platform_window_should_close()) {
		if (Platform_is_key_down(KEY_D)) {
			players[my_id_idx].center.x += 750 * Platform_get_frame_time();
		}
		if (Platform_is_key_down(KEY_A)) {
			players[my_id_idx].center.x -= 750 * Platform_get_frame_time();
		}
		if (Platform_is_key_down(KEY_W)) {
			players[my_id_idx].center.y -= 750 * Platform_get_frame_time();
		}
		if (Platform_is_key_down(KEY_S)) {
			players[my_id_idx].center.y += 750 * Platform_get_frame_time();
		}

		Block *collision = NULL;

		if ((collision = QuadTree_check_collision(root, (AABB) { .center = players[my_id_idx].center, .half_dimension = GRID_SIZE }))) {
			printf("Collision: %.2f %.2f\n", collision->center.x, collision->center.y);
		}
		printf("Collision: %p\n", collision);

		Platform_begin_drawing();
			Platform_clear_background(0xfffbfbfb);
			QuadTree_display(root);

			for (u32 i = 0; i < 3; i++) {
				Platform_draw_rectangle(
						players[i].center.x-(float)GRID_SIZE,
						players[i].center.y-(float)GRID_SIZE,
						GRID_SIZE, GRID_SIZE, 0xff919191);
			}
		Platform_end_drawing();
	}

	Platform_close_window();

	return 0;
}
