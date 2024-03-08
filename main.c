#include "defines.h"
#include "mm.h"
#include "quadtree.h"
#include "./platform.h"
#include "types.h"
#include "levels/level.h"

int main()
{
	QuadTree *root = QuadTree_new((float)HALF_WIDTH, (float)HALF_HEIGHT, (float)HALF_WIDTH);

	u32 blocks_size = 0;
	Block *grid = (Block*)level_bytes;

	for (u64 i = 0; i < size ; i++) {
		QuadTree_insert(root, grid[i]);
	}

	Point players[] = {
		{ GRID_SIZE*4 , GRID_SIZE*4  },
		{ GRID_SIZE*1 , GRID_SIZE*4  },
		{ GRID_SIZE*13, GRID_SIZE*13 },
	};

	mm_log();

	Platform_init_window("Bomb royale", SCREEN_WIDTH, SCREEN_HEIGHT);

	while (!Platform_window_should_close()) {
		Platform_begin_drawing();
			QuadTree_display(root);

			for (u32 i = 0; i < 3; i++) {
				Platform_draw_rectangle(
						players[i].x-(float)GRID_SIZE,
						players[i].y-(float)GRID_SIZE,
						GRID_SIZE, GRID_SIZE, 0xff919191);
			}
		Platform_end_drawing();
	}

	Platform_close_window();

	return 0;
}
