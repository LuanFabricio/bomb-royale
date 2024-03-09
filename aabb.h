#ifndef AABB_H_
#define AABB_H_

#include "./types.h"
#include "./defines.h"
#include "platform.h"

#define AABB_PREFIX static

AABB_PREFIX boolean AABB_contains_point(AABB aabb, Point p)
{
	float left 	= aabb.center.x - aabb.half_dimension;
	float right 	= aabb.center.x + aabb.half_dimension;
	float top 	= aabb.center.y - aabb.half_dimension;
	float bottom 	= aabb.center.y + aabb.half_dimension;

	return left <= p.x && right >= p.x
		&& top <= p.y && bottom >= p.y;
}

AABB_PREFIX  boolean AABB_intersects(AABB aabb1, AABB aabb2)
{
	float aabb1_left 	= aabb1.center.x - aabb1.half_dimension;
	float aabb1_right 	= aabb1.center.x + aabb1.half_dimension;
	float aabb1_top 	= aabb1.center.y - aabb1.half_dimension;
	float aabb1_bottom 	= aabb1.center.y + aabb1.half_dimension;

	float aabb2_left 	= aabb2.center.x - aabb2.half_dimension;
	float aabb2_right 	= aabb2.center.x + aabb2.half_dimension;
	float aabb2_top 	= aabb2.center.y - aabb2.half_dimension;
	float aabb2_bottom 	= aabb2.center.y + aabb2.half_dimension;

	return (aabb1_left <= aabb2_right && aabb1_right >= aabb2_left)
		&& (aabb1_top <= aabb2_bottom && aabb1_bottom >= aabb2_top);
}

AABB_PREFIX void AABB_display(const AABB aabb)
{
	float left = aabb.center.x - aabb.half_dimension;
	float top = aabb.center.y - aabb.half_dimension;

	float dimension = aabb.half_dimension * 2.0;
	Platform_draw_rectangle_lines(left, top, dimension, dimension, AABB_BORDER_COLOR);
}

#endif // AABB_H_
