#include "raylib.h"

#include "../defines.h"
#include "../types.h"
#include "../utils/levels.h"
#include "../utils/colors.h"

void draw_grid()
{
	for (u32 i = 0; i < GRID_LENGTH; i++) {
		u32 x = i * GRID_SIZE;
		DrawLine(x, 0, x, SCREEN_HEIGHT, BLACK);

		u32 y = i * GRID_SIZE;
		DrawLine(0, y, SCREEN_WIDTH, y, BLACK);
	}
}

Color get_color_by_grid_type(GridType gt)
{
	u32 color = Color_get_color_by_grid_type(gt);
	return *(Color*)&color;
	// const Color colors[] = {
	// 	RAYWHITE,
	// 	RED,
	// 	GRAY,
	// };
        //
	// return colors[gt % 3];
}

void remove_block_by_point(Point p, Block *blocks, u32 *block_size)
{
	for (u32 i = 0; i < *block_size; i++) {
		if (p.x == blocks[i].center.x && p.y == blocks[i].center.y) {
			*block_size -= 1;
			blocks[i] = blocks[*block_size];
			return;
		}
	}
}

#define LEVEL_BASE_PATH "./levels/"

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Level editor");

	SetTargetFPS(60);
	Vector2 mouse_pos = {0};

	Block blocks[GRID_LENGTH*GRID_LENGTH] = {AIR};
	u32 blocks_size = 0;

	load_grid_by_bin(LEVEL_BASE_PATH "level.bin", blocks, &blocks_size);

	GridType current_type = DESTRUCTIBLE;

	u32 block_uid = 0;

	while(!WindowShouldClose()) {
		if (IsKeyPressed(KEY_ONE)) {
			current_type = AIR;
		} else if (IsKeyPressed(KEY_TWO)) {
			current_type = DESTRUCTIBLE;
		} else if (IsKeyPressed(KEY_THREE)) {
			current_type = INDESTRUCTIBLE;
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			mouse_pos = GetMousePosition();

			u32 mouse_x = (u32)mouse_pos.x / GRID_SIZE + 1;
			u32 mouse_y = (u32)mouse_pos.y / GRID_SIZE + 1;
			mouse_x *= GRID_SIZE;
			mouse_y *= GRID_SIZE;
			Point p = {
				.x = mouse_x,
				.y = mouse_y,
			};

			if (current_type != AIR) {
				blocks[blocks_size].uid = block_uid++;
				blocks[blocks_size].center = p;
				blocks[blocks_size++].grid_type = current_type;
			} else {
				remove_block_by_point(p, blocks, &blocks_size);
			}
		}

		BeginDrawing();
			ClearBackground(RAYWHITE);

			for (u32 i = 0; i < blocks_size; i++) {
				Color c = get_color_by_grid_type(blocks[i].grid_type);
				DrawRectangle(
						blocks[i].center.x-(float)GRID_SIZE,
						blocks[i].center.y-(float)GRID_SIZE,
						GRID_SIZE, GRID_SIZE, c);
			}
			draw_grid();
		EndDrawing();
	}

	save_grid_as_bin(LEVEL_BASE_PATH"level.bin", blocks, &blocks_size);
	save_grid_as_header(LEVEL_BASE_PATH"level.h", (u8*)&blocks, blocks_size);

	CloseWindow();
}
