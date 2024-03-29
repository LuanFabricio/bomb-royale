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

	Bomb bomb_arr[GRID_LENGTH * GRID_LENGTH] = {0};
	u8 bomb_size = 0;

	Fire fire_arr[GRID_LENGTH * GRID_LENGTH] = {0};
	u8 fire_size = 0;

	mm_log();

	printf("QuadTree len: %llu\n", QuadTree_get_len(root));

	Platform_init_window("Bomb royale", SCREEN_WIDTH, SCREEN_HEIGHT);

	while (!Platform_window_should_close()) {
		for (u32 i = 0; i < bomb_size; i++) {
			if (bomb_arr[i].bomb_item.tick_to_explode > 0) bomb_arr[i].bomb_item.tick_to_explode -= 1;
			else {
				AABB fire_aabb = {
					.center = bomb_arr[i].bomb_item.center,
					.half_dimension = GRID_SIZE - 2,
				};

				Point fire_centers[] = {
					fire_aabb.center,
					fire_aabb.center,
					fire_aabb.center,
					fire_aabb.center,
				};

				fire_centers[0].x -= GRID_SIZE;
				fire_centers[1].x += GRID_SIZE;
				fire_centers[2].y -= GRID_SIZE;
				fire_centers[3].y += GRID_SIZE;

				printf("Bomb %i spawning fire...\n", i);
				Block *collision_block = NULL;
				for (u8 j = 0; j < 4; j++) {
					fire_aabb.center = fire_centers[j];
					if ((collision_block = QuadTree_check_collision(root, fire_aabb)) != NULL && collision_block->grid_type != AIR) {
						printf("Collides with %i\n", collision_block->grid_type);
						printf("\t%.2f %.2f\n", collision_block->center.x, collision_block->center.y);
						if (collision_block->grid_type == DESTRUCTIBLE) {
							collision_block->grid_type = AIR;
						}
					} else {
						fire_arr[fire_size].fire_item.center = fire_aabb.center;
						fire_arr[fire_size++].fire_item.tick_to_explode = FIRE_NORMAL_TICKS;
						printf("Adding fire!\n");
					}
				}

				bomb_size--;
				bomb_arr[i] = bomb_arr[bomb_size];
				--i;
			}
		}

		for (u8 i = 0; i < fire_size; i++) {
			if (fire_arr[i].fire_item.tick_to_explode > 0) fire_arr[i].fire_item.tick_to_explode -= 1;
			else {
				fire_size--;
				fire_arr[i] = fire_arr[fire_size];
			}
		}

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

		if (Platform_is_key_down(BR_KEY_SPACE)) {
			bomb_arr[bomb_size] = (Bomb) {
				.bomb_item = {
					.center = players[my_id_idx].center,
					.size = 3,
					.tick_to_explode = BOMB_NORMAL_TICKS
				}
			};
			bomb_arr[bomb_size].bomb_item.center.x = ((int)players[my_id_idx].center.x / GRID_SIZE) * GRID_SIZE;
			bomb_arr[bomb_size].bomb_item.center.y = ((int)players[my_id_idx].center.y / GRID_SIZE) * GRID_SIZE;
			bomb_size++;
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

			for (u8 i = 0; i < bomb_size; i++) {
				Platform_draw_rectangle(
						bomb_arr[i].bomb_item.center.x-(float)GRID_SIZE,
						bomb_arr[i].bomb_item.center.y-(float)GRID_SIZE,
						GRID_SIZE, GRID_SIZE,
						0xff151515);
			}

			for (u8 i = 0; i < fire_size; i++) {
				Platform_draw_rectangle(
						fire_arr[i].fire_item.center.x-(float)GRID_SIZE,
						fire_arr[i].fire_item.center.y-(float)GRID_SIZE,
						GRID_SIZE, GRID_SIZE,
						0xff0000ff);
			}
		Platform_end_drawing();
	}

	Platform_close_window();

	return 0;
}
