#include "raylib.h"

#include "../defines.h"
#include "../types.h"
#include "../utils/levels.h"

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
	const Color colors[] = {
		RAYWHITE,
		RED,
		GRAY,
	};

	return colors[gt % 3];
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

			u32 mouse_x = (u32)mouse_pos.x / GRID_SIZE;
			u32 mouse_y = (u32)mouse_pos.y / GRID_SIZE;

			blocks[blocks_size].x = mouse_x;
			blocks[blocks_size].y = mouse_y;
			blocks[blocks_size++].grid_type = current_type;
		}

		BeginDrawing();
			ClearBackground(RAYWHITE);

			for (u32 i = 0; i < blocks_size; i++) {
				Color c = get_color_by_grid_type(blocks[i].grid_type);
				DrawRectangle(blocks[i].x*GRID_SIZE, blocks[i].y*GRID_SIZE,
						GRID_SIZE, GRID_SIZE, c);
			}
			draw_grid();
		EndDrawing();
	}

	save_grid_as_bin(LEVEL_BASE_PATH"level.bin", blocks, &blocks_size);
	save_grid_as_header(LEVEL_BASE_PATH"level.h", (u8*)&blocks, blocks_size);

	CloseWindow();
}
