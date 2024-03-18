#ifndef UTILS_H_
#define UTILS_H_

static float minf(float x, float y)
{
	return x > y ? y : x;
}

static float maxf(float x, float y)
{
	return x < y ? y : x;
}

#endif // UTILS_H_
