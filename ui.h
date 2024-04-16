#ifndef UI_H_
#define UI_H_

#include "./platform.h"
#include "./defines.h"

static void UI_draw_home()
{
	const float TITLE_WIDTH = 550;
	const float TITLE_HEIGHT = 125;

	float title_x = (float)HALF_WIDTH - TITLE_WIDTH / 2.0;
	float title_y = SCREEN_HEIGHT * 0.125 - TITLE_HEIGHT / 2.0;

	Platform_draw_rectangle(title_x, title_y, TITLE_WIDTH, TITLE_HEIGHT, 0xff999999);

	Platform_text("Teste...", title_x, title_y, 32, 0xff000000);
}

#endif // UI_H_
