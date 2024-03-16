#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "./platform.h"
#include "./types.h"
#include "./mm.h"
#include "utils/colors.h"

#define QUADTREE static

#include "./defines.h"
#include "./aabb.h"

QUADTREE QuadTree *QuadTree_new(float x, float y, float half_dimension)
{
	AABB node = {
		.center = {
			.x = x,
			.y = y,
		},
		.half_dimension = half_dimension,
	};

	QuadTree *q_ptr = (QuadTree*) mm_malloc(sizeof(QuadTree));
	if (q_ptr) {
		q_ptr->node = node;
		q_ptr->objects_size = 0;
		for (unsigned int i = 0; i < NODE_CAPACITY; i++) {
			q_ptr->objects[i].center.x = 0;
			q_ptr->objects[i].center.y = 0;
			q_ptr->objects[i].grid_type = AIR;
		}
		q_ptr->north_west = NULL;
		q_ptr->north_east = NULL;
		q_ptr->south_west = NULL;
		q_ptr->south_east = NULL;

		return q_ptr;
	}

	return NULL;
}

QUADTREE void QuadTree_subdivide(QuadTree *root)
{
	float x, y, half_dimension = root->node.half_dimension / 2.0;
	x = root->node.center.x - half_dimension;
	y = root->node.center.y - half_dimension;
	root->north_west = QuadTree_new(x, y, half_dimension);

	x = root->node.center.x + half_dimension;
	y = root->node.center.y - half_dimension;
	root->north_east = QuadTree_new(x, y, half_dimension);

	x = root->node.center.x - half_dimension;
	y = root->node.center.y + half_dimension;
	root->south_west = QuadTree_new(x, y, half_dimension);

	x = root->node.center.x + half_dimension;
	y = root->node.center.y + half_dimension;
	root->south_east = QuadTree_new(x, y, half_dimension);
}

QUADTREE boolean QuadTree_insert(QuadTree *root, Block obj)
{
	if (!AABB_contains_point(root->node, obj.center)) {
		return false;
	}

	if (root->objects_size < NODE_CAPACITY && root->north_west == NULL) {
		root->objects[root->objects_size] = obj;
		root->objects_size += 1;
		return true;
	}

	if(root->north_west == NULL) {
		QuadTree_subdivide(root);
	}

	if (QuadTree_insert(root->north_west, obj)) return true;
	if (QuadTree_insert(root->north_east, obj)) return true;
	if (QuadTree_insert(root->south_west, obj)) return true;
	if (QuadTree_insert(root->south_east, obj)) return true;

	return false;
}

QUADTREE Block* QuadTree_check_collision(const QuadTree *root, AABB aabb)
{
	if (!AABB_intersects(root->node, aabb)) {
		return NULL;
	}

	for (u8 i = 0; i < root->objects_size; i++) {
		if (AABB_contains_point(aabb, root->objects[i].center)) {
			return (Block*)&root->objects[i];
		}
	}

	if (root->north_east == NULL) {
		return NULL;
	}

	Block *block = NULL;

	if ((block = QuadTree_check_collision(root->north_west, aabb))) return block;
	if ((block = QuadTree_check_collision(root->north_east, aabb))) return block;
	if ((block = QuadTree_check_collision(root->south_west, aabb))) return block;
	if ((block = QuadTree_check_collision(root->south_east, aabb))) return block;

	return NULL;
}

QUADTREE void QuadTree_display(const QuadTree *root)
{
	if (root == NULL) return;

	u32 color = 0;
	GridType gt = AIR;
	for (u32 i = 0; i < root->objects_size; i++) {
		gt = root->objects[i].grid_type;

		if (gt != AIR ){
		color = Color_get_color_by_grid_type(gt);
			Platform_draw_rectangle(
					root->objects[i].center.x - (float)GRID_SIZE,
					root->objects[i].center.y - (float)GRID_SIZE,
					GRID_SIZE, GRID_SIZE, color);
		}
	}

	QuadTree_display(root->north_west);
	QuadTree_display(root->north_east);
	QuadTree_display(root->south_west);
	QuadTree_display(root->south_east);

	// AABB_display(root->node);
}

QUADTREE u64 QuadTree_get_len(const QuadTree *root)
{
	u64 len = root->objects_size;

	if (root->north_west) {
		len += QuadTree_get_len(root->north_west) + 1;
		len += QuadTree_get_len(root->north_east) + 1;
		len += QuadTree_get_len(root->south_west) + 1;
		len += QuadTree_get_len(root->south_east) + 1;
	}

	return len;
}

#endif // QUADTREE_H_
