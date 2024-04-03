#include "defines.h"
#include "mm.h"
#include "quadtree.h"
#include "./platform.h"
#include "types.h"
#include "levels/level.h"
#include "utils/bomb.h"
#include "utils/collision_handler.h"
#include "utils/vector.h"
#include "utils/fire.h"
// #include <stdio.h>

static QuadTree *root;

static u32 blocks_size = 0;
static Block *grid = NULL;

static Player players[] = {
	{ .id = 1, .center = (Point){ GRID_SIZE*1 , GRID_SIZE*1  }, .fire_power_up = 1, .alive = true },
	{ .id = 2, .center = (Point){ GRID_SIZE*13, GRID_SIZE*1  }, .fire_power_up = 1, .alive = true },
	{ .id = 3, .center = (Point){ GRID_SIZE*13, GRID_SIZE*13 }, .fire_power_up = 1, .alive = true },
};
static u8 my_id = 1;
static u8 my_id_idx = 0;

static u8 players_len = sizeof(players)/sizeof(players[0]);

static Vector2 speed = {0};

static BombArray bombs = {
	.arr = {0}, .size = 0,
};
static u8 bomb_delay = 0;

static FireArray fires = {
	.arr = {0}, .size = 0,
};

void game_loop()
{
	bombs.size = Bomb_tick(root, &bombs, &fires);

	fires.size = Fire_tick(fires.arr, fires.size);

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

	if (bomb_delay == 0 && Platform_is_key_down(BR_KEY_SPACE)) {
		bombs.arr[bombs.size] = (Bomb) {
			.bomb_item = {
				.center = players[my_id_idx].center,
				.size = 3,
					.tick_to_explode = BOMB_NORMAL_TICKS
				},
				.fire_power = minu8(players[my_id_idx].fire_power_up, GRID_LENGTH),
			};
			bombs.arr[bombs.size].bomb_item.center.x = ((int)players[my_id_idx].center.x / GRID_SIZE) * GRID_SIZE;
			bombs.arr[bombs.size].bomb_item.center.y = ((int)players[my_id_idx].center.y / GRID_SIZE) * GRID_SIZE;
			bombs.size++;
			bomb_delay = BOMB_DELAY_TICK;
		} else if (bomb_delay > 0) {
			bomb_delay--;
		}

		if (Platform_is_key_pressed(BR_KEY_EQUAL)) {
			players[my_id_idx].fire_power_up += 1;
		}

		Platform_begin_drawing();
			Platform_clear_background(0xfffbfbfb);
			QuadTree_display(root);

			for (u32 i = 0; i < players_len; i++) {
				Platform_draw_rectangle(
						players[i].center.x-(float)GRID_SIZE,
						players[i].center.y-(float)GRID_SIZE,
						GRID_SIZE, GRID_SIZE, 0xff919191);
			}

			for (u8 i = 0; i < bombs.size; i++) {
				Platform_draw_rectangle(
						bombs.arr[i].bomb_item.center.x-(float)GRID_SIZE,
						bombs.arr[i].bomb_item.center.y-(float)GRID_SIZE,
						GRID_SIZE, GRID_SIZE,
						0xff151515);
			}

			for (u8 i = 0; i < fires.size; i++) {
				Platform_draw_rectangle(
						fires.arr[i].fire_item.center.x-(float)GRID_SIZE,
						fires.arr[i].fire_item.center.y-(float)GRID_SIZE,
						GRID_SIZE, GRID_SIZE,
						0xff0000ff);
			}
		Platform_end_drawing();
}

int main()
{
	root = QuadTree_new((float)HALF_WIDTH, (float)HALF_HEIGHT, (float)HALF_WIDTH);
	grid = (Block*)level_bytes;
	for (u64 i = 0; i < size ; i++) {
		// TODO: Fix this hack
		// NOTE: level editor saves with 0 index
		// and the game uses 1 index
		grid[i].center.x += GRID_SIZE;
		grid[i].center.y += GRID_SIZE;
		QuadTree_insert(root, grid[i]);
	}

	for (u8 i = 0; i < players_len; i++) {
		if (players[i].id == my_id_idx) {
			my_id_idx = i;
			break;
		}
	}

#ifndef PLATFORM_WEB
	mm_log();
#endif // PLATFORM_WEB

	Platform_init_window("Bomb royale", SCREEN_WIDTH, SCREEN_HEIGHT);

#ifndef PLATFORM_WEB
	while (!Platform_window_should_close()) {
		game_loop();
	}

	Platform_close_window();
#else
	Platform_set_loop(game_loop);
#endif // PLATFORM_WEB

	return 0;
}
