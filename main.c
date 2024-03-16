#include "defines.h"
#include "mm.h"
#include "quadtree.h"
#include "./platform.h"
#include "types.h"
#include "levels/level.h"
#include "utils/collision_handler.h"
#include "utils/vector.h"
#include <stdio.h>

int main()
{
	QuadTree *root = QuadTree_new((float)HALF_WIDTH, (float)HALF_HEIGHT, (float)HALF_WIDTH);

	u32 blocks_size = 0;
	Block *grid = (Block*)level_bytes;

	for (u64 i = 0; i < size ; i++) {
		// TODO: Fix this hack
		// NOTE: level editor saves with 0 index
		// and the game uses 1 index
		grid[i].center.x += GRID_SIZE;
		grid[i].center.y += GRID_SIZE;
		QuadTree_insert(root, grid[i]);
	}

	Player players[] = {
		{ .id = 1, .center = (Point){ GRID_SIZE*1 , GRID_SIZE*1  }, },
		{ .id = 2, .center = (Point){ GRID_SIZE*13, GRID_SIZE*1  }, },
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

	Vector2 speed = {0};

	mm_log();

	printf("QuadTree len: %llu\n", QuadTree_get_len(root));

	Platform_init_window("Bomb royale", SCREEN_WIDTH, SCREEN_HEIGHT);

	while (!Platform_window_should_close()) {
		if (Platform_is_key_down(BR_KEY_D)) {
			speed.x = 1;
		} else if (Platform_is_key_down(BR_KEY_A)) {
			speed.x = -1;
		} else {
			speed.x = 0;
		}

		if (Platform_is_key_down(BR_KEY_W)) {
			speed.y = -1;
		} else if (Platform_is_key_down(BR_KEY_S)) {
			speed.y = 1;
		} else {
			speed.y = 0;
		}

		speed = Vector2_scale(Vector2_normalize(speed), PLAYER_SPEED * Platform_get_frame_time());
		players[my_id_idx].center = HandleCollision_player_collision(root, players, players_len, my_id_idx, speed);

		Platform_begin_drawing();
			Platform_clear_background(0xfffbfbfb);
			QuadTree_display(root);

			for (u32 i = 0; i < players_len; i++) {
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
