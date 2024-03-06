#include "defines.h"
#include "mm.h"
#include "quadtree.h"
#include "./platform.h"
#include "types.h"
#include "utils/levels.h"

int main()
{
	QuadTree *root = QuadTree_new((float)HALF_WIDTH, (float)HALF_HEIGHT, (float)HALF_WIDTH);

	u32 blocks_size = 0;
	Block grid[GRID_LENGTH*GRID_LENGTH] = {0};
	load_grid_by_bin("./levels/level.bin", grid, &blocks_size);

	for (u64 i = 0; i < blocks_size; i++) {
		QuadTree_insert(root, grid[i]);
	}

	// QuadTree_insert(root, (Block) { .aabb = { .center = { 400.0, 300.0, }, .half_dimension = 42.0f }, .grid_type = AIR } );
	// QuadTree_insert(root, (Block) { .aabb = { .center = { 200.0, 300.0, }, .half_dimension = 42.0f }, .grid_type = DESTRUCTIBLE } );
	// QuadTree_insert(root, (Block) { .aabb = { .center = { 100.0, 300.0, }, .half_dimension = 42.0f }, .grid_type = INDESTRUCTIBLE } );
	// QuadTree_insert(root, (Block) { .aabb = { .center = { 400.0, 500.0, }, .half_dimension = 42.0f }, .grid_type = INDESTRUCTIBLE } );

	mm_log();

	Platform_init_window("Bomb", SCREEN_WIDTH, SCREEN_HEIGHT);

	while (!Platform_window_should_close()) {
		Platform_begin_drawing();
			QuadTree_display(root);
		Platform_end_drawing();
	}

	Platform_close_window();

	return 0;
}
