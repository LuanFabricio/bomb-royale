#ifndef FIRE_H_
#define FIRE_H_

#include "../types.h"
#include "../quadtree.h"

static void Fire_calc_position(Point* center, u8 idx, u8 axis)
{

	switch (axis) {
		case 0:
			center->x -= GRID_SIZE * (idx + 1);
			break;
		case 1:
			center->x += GRID_SIZE * (idx + 1);
			break;
		case 2:
			center->y -= GRID_SIZE * (idx + 1);
			break;
		case 3:
			center->y += GRID_SIZE * (idx + 1);
			break;
	}
}

static boolean Fire_handle_collision(Fire* fire_arr, u8 *fire_size, QuadTree *root, AABB fire_aabb)
{
	boolean collide = false;
	Block *collision_block = QuadTree_check_collision(root, fire_aabb);

	if (collision_block != NULL && collision_block->grid_type != AIR) {
		if (collision_block->grid_type == DESTRUCTIBLE) {
			collision_block->grid_type = AIR;
		}
		collide = true;
	}

	if (collision_block == NULL || collision_block->grid_type == DESTRUCTIBLE) {
		fire_arr[*fire_size].fire_item.center = fire_aabb.center;
		fire_arr[*fire_size].fire_item.tick_to_explode = FIRE_NORMAL_TICKS;
		*fire_size += 1;
	}

	return collide;
}

#endif // FIRE_H_
