#include "defines.h"
#include "mm.h"
#include "quadtree.h"
#include "./platform.h"
#include "types.h"
#include "utils/levels.h"
#include "levels/level.h"
#include <stdio.h>

int main()
{
	QuadTree *root = QuadTree_new((float)HALF_WIDTH, (float)HALF_HEIGHT, (float)HALF_WIDTH);

	u32 blocks_size = 0;
	// Block grid[GRID_LENGTH*GRID_LENGTH] = level_bytes;
	Block *grid = (Block*)level_bytes;
	// load_grid_by_bin("./levels/level.bin", grid, &blocks_size);

	printf("Size: %u\n", size);
	for (u64 i = 0; i < size ; i++) {
		printf("[%llu] {%.2f %.2f} %.2f %u\n", i, grid[i].center.x, grid[i].center.y, GRID_SIZE/2.0, grid[i].grid_type);
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
