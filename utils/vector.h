#ifndef VECTOR_H_
#define VECTOR_H_

#include "../types.h"
#include "../platform.h"

#define VECTOR static

VECTOR Point Vector2_add_point(Vector2 v, Point p)
{

	p.x += v.x;
	p.y += v.y;

	return p;
}

VECTOR Vector2 Vector2_add(Vector2 v1, Vector2 v2)
{
	v1.x += v2.x;
	v1.y += v2.y;

	return v1;
}


VECTOR Vector2 Vector2_scale(Vector2 v, float scale)
{
	v.x *= scale;
	v.y *= scale;

	return v;
}

VECTOR Vector2 Vector2_normalize(Vector2 v)
{
	float length = v.x * v.x + v.y * v.y;
	length = Platform_sqrtf(length);

	if (length == 0) {
		return v;
	}

	v.x /= length;
	v.y /= length;

	return v;
}

#endif // VECTOR_H_
