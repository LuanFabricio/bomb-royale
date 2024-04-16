#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

#include "raylib.h"

#define U32_TO_COLOR(u32_color) *(Color*)(&(u32_color))

Font font = {0};

void Platform_init_window(const char *title, uint32_t w, uint32_t h)
{
	InitWindow(w, h, title);
	SetTargetFPS(60);

	font = LoadFont("../../assets/Jersey/Jersey10-Regular.ttf");
}

bool Platform_window_should_close()
{
	return WindowShouldClose();
}

void Platform_begin_drawing()
{
	BeginDrawing();
}

void Platform_end_drawing()
{
	EndDrawing();
}

void Platform_clear_background(uint32_t color)
{
	ClearBackground(U32_TO_COLOR(color));
}

void Platform_draw_rectangle(float x, float y, float w, float h, uint32_t color)
{
	DrawRectangle(x, y, w, h, U32_TO_COLOR(color));
}

void Platform_draw_rectangle_lines(float x, float y, float w, float h, uint32_t color)
{
	DrawRectangleLines(x, y, w, h, U32_TO_COLOR(color));
}

void Platform_text(const char *text, float x, float y, uint32_t font_size, uint32_t color)
{
	uint32_t default_font_size = 10;
	if (font_size < default_font_size) font_size = default_font_size;
	float spacing = (float)font_size / default_font_size;

	DrawTextEx(font, text, (Vector2){ .x = x, .y = y }, font_size, spacing, U32_TO_COLOR(color));
	// DrawText(text, x, y, font_size, U32_TO_COLOR(color));
}

bool Platform_is_key_pressed(uint32_t key)
{
	return IsKeyPressed(key);
}

bool Platform_is_key_down(uint32_t key)
{
	bool is_key_down = IsKeyDown(key);
	return is_key_down;
}

float Platform_get_frame_time()
{
	return GetFrameTime();
}

void Platform_close_window()
{
	CloseWindow();
}

void Platform_rand_seed()
{
	srand(time(NULL));
}

float Platform_rand_float_range(float min, float max)
{
	float r = (float)rand() / (float)INT_MAX;
	return min + r * (max - min);
}

float Platform_sqrtf(float x)
{
	return sqrtf(x);
}
