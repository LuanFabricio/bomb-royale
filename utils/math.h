#ifndef UTILS_H_
#define UTILS_H_

#include "../types.h"

static float minf(float x, float y)
{
	return x > y ? y : x;
}

static float maxf(float x, float y)
{
	return x < y ? y : x;
}

static u8 maxu8(u8 x, u8 y)
{
	return x < y ? y : x;
}

static u8 minu8(u8 x, u8 y)
{
	return x < y ? x : y;
}

#endif // UTILS_H_
