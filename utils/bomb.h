#ifndef BOMB_H_
#define BOMB_H_

#include "../types.h"
#include "../quadtree.h"
#include "./fire.h"

static u32 Bomb_tick(QuadTree *root, BombArray *bombs, FireArray *fires)
{
	for (u32 i = 0; i < bombs->size; i++) {
		if (bombs->arr[i].bomb_item.tick_to_explode > 0) bombs->arr[i].bomb_item.tick_to_explode -= 1;
		else {
			AABB fire_aabb = {
				.center = bombs->arr[i].bomb_item.center,
				.half_dimension = GRID_SIZE - 2,
			};
			fires->arr[fires->size].fire_item.center = fire_aabb.center;
			fires->arr[fires->size++].fire_item.tick_to_explode = FIRE_NORMAL_TICKS;

			Point fire_center_top[MAX_FIRE_LINE] = {0};
			Point fire_center_left[MAX_FIRE_LINE] = {0};
			Point fire_center_right[MAX_FIRE_LINE] = {0};
			Point fire_center_bottom[MAX_FIRE_LINE] = {0};

			for (u8 j = 0; j < bombs->arr[i].fire_power; j++) {
				fire_center_top[j] 	= fire_aabb.center;
				fire_center_left[j] 	= fire_aabb.center;
				fire_center_right[j]	= fire_aabb.center;
				fire_center_bottom[j] 	= fire_aabb.center;

				Fire_calc_position(&fire_center_top[j], j, 2);
				Fire_calc_position(&fire_center_left[j], j, 0);
				Fire_calc_position(&fire_center_right[j], j, 1);
				Fire_calc_position(&fire_center_bottom[j], j, 3);

			}

			Point* directions_avaiable[] = {
				fire_center_left, fire_center_right,
				fire_center_top, fire_center_bottom,
			};

			for (u8 j = 0; j < 4; j++) {
				for (u8 k = 0; k < bombs->arr[i].fire_power; k++) {
					fire_aabb.center = directions_avaiable[j][k];
					if (Fire_handle_collision(fires, root, fire_aabb)) {
						break;
					}
				}
			}

			bombs->size--;
			bombs->arr[i] = bombs->arr[bombs->size];
			--i;
		}
	}
	return bombs->size;
}

#endif // BOMB_H_
