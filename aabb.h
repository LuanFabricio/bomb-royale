#ifndef AABB_H_
#define AABB_H_

#include "./types.h"
#include "./defines.h"
#include "platform.h"

#define AABB_PREFIX static

AABB_PREFIX boolean AABB_contains_point(AABB aabb, Point p)
{
	float left = aabb.center.x - aabb.half_dimension;
	float right = aabb.center.x + aabb.half_dimension;
	float top = aabb.center.y - aabb.half_dimension;
	float bottom = aabb.center.y + aabb.half_dimension;

	return left <= p.x && right >= p.x
		&& top <= p.y && bottom >= p.y;
}

AABB_PREFIX void AABB_display(const AABB aabb)
{
	float left = aabb.center.x - aabb.half_dimension;
	float top = aabb.center.y - aabb.half_dimension;

	float dimension = aabb.half_dimension * 2.0;
	Platform_draw_rectangle_lines(left, top, dimension, dimension, AABB_BORDER_COLOR);
}

#endif // AABB_H_
