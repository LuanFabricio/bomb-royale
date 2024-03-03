#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "./platform.h"
#include "./types.h"
#include "./mm.h"

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
			q_ptr->objects[i].half_dimension = 0;
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

QUADTREE boolean QuadTree_insert(QuadTree *root, AABB obj)
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

QUADTREE void QuadTree_display(const QuadTree *root)
{
	if (root == NULL) return;

	AABB_display(root->node);

	float half = 0;
	for (u32 i = 0; i < root->objects_size; i++) {
		half = root->objects[i].half_dimension;
		Platform_draw_rectangle(
				root->objects[i].center.x - half,
				root->objects[i].center.y - half,
				half * 2.0, half * 2.0,	POINT_COLOR);
	}

	QuadTree_display(root->north_west);
	QuadTree_display(root->north_east);
	QuadTree_display(root->south_west);
	QuadTree_display(root->south_east);
}

#endif // QUADTREE_H_
