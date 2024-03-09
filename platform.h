#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "./types.h"

// ========================== Window section =========================

extern void Platform_init_window(const char* title, u32 w, u32 h);
extern void Platform_close_window();
extern boolean Platform_window_should_close();

// ========================== Draw section  ==========================

extern void Platform_begin_drawing();
extern void Platform_end_drawing();
extern void Platform_clear_background(u32 color);

extern void Platform_draw_rectangle(float x, float y, float w, float h, u32 color);
extern void Platform_draw_rectangle_lines(float x, float y, float w, float h, u32 color);

// ========================== Input section  =========================

extern boolean Platform_is_key_pressed(u32 key);
extern boolean Platform_is_key_down(u32 key);

// ========================== Random section =========================

extern float Platform_rand_seed();
extern float Platform_rand_float_range(float min, float max);

// ======================== Game loop section ========================

extern float Platform_get_frame_time();
extern void Platform_set_loop(void* loop_fn);

#endif // PLATFORM_H_
