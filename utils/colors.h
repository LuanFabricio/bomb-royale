#ifndef COLORS_H_
#define COLORS_H_

#include "../types.h"

const u32 COLORS[] = {
	0xffffffff,
	0xff0000ff,
	0xffbbbbbb,
};
const u64 COLOR_SIZE = sizeof(COLORS)/sizeof(COLORS[0]);

static u32 Color_get_color_by_grid_type(GridType gt)
{
	return COLORS[gt % COLOR_SIZE];
}

#endif // COLORS_H_
