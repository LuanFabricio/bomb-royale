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

static boolean Fire_handle_collision(FireArray* fires, QuadTree *root, AABB fire_aabb)
{
	Block *collision_block = QuadTree_check_collision(root, fire_aabb);
	boolean collide = collision_block != NULL && collision_block->grid_type != AIR;

	if (collision_block == NULL || collision_block->grid_type == DESTRUCTIBLE) {
		if (collision_block) {
			collision_block->grid_type = AIR;
			QuadTree_remove_by_id(root, collision_block->uid);
		}

		fires->arr[fires->size].fire_item.center = fire_aabb.center;
		fires->arr[fires->size].fire_item.tick_to_explode = FIRE_NORMAL_TICKS;
		fires->size += 1;
	}

	return collide;
}

static void Fire_tick(FireArray *fires)
{
	for (u8 i = 0; i < fires->size; i++) {
		if (fires->arr[i].fire_item.tick_to_explode > 0) {
			fires->arr[i].fire_item.tick_to_explode -= 1;
		} else {
			fires->arr[i] = fires->arr[fires->size - 1];
			fires->size -= 1;
			if (i > 0) i -= 1;
		}
	}
}

#endif // FIRE_H_
