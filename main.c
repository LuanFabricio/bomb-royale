#include "mm.h"
#include "quadtree.h"
#include "./platform.h"

int main()
{
	QuadTree *root = QuadTree_new((float)HALF_WIDTH, (float)HALF_HEIGHT, (float)HALF_WIDTH);

	QuadTree_insert(root, (AABB) { .center = { 400.0, 300.0, }, .half_dimension = 42.0f });
	QuadTree_insert(root, (AABB) { .center = { 200.0, 300.0, }, .half_dimension = 42.0f });
	QuadTree_insert(root, (AABB) { .center = { 100.0, 300.0, }, .half_dimension = 42.0f });
	QuadTree_insert(root, (AABB) { .center = { 400.0, 500.0, }, .half_dimension = 42.0f });

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
