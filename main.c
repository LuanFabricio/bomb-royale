#include "defines.h"
#include "mm.h"
#include "quadtree.h"
#include "types.h"
#include "levels/level.h"
#include "./platform.h"
#include "gamemode/default.h"
#include "utils/bomb.h"
#include "utils/collision_handler.h"
#include "utils/input.h"
#include "utils/vector.h"
#include "utils/fire.h"

static Game game = {0};

void game_loop()
{
	game.bombs.size = Bomb_tick(game.root, &game.bombs, &game.fires);

	game.fires.size = Fire_tick(&game.fires);

	Input_place_bomb(&game.bombs, game.players, &game.bomb_delay, game.my_id_idx);

	Vector2 speed = Input_speed();
	speed = Vector2_scale(Vector2_normalize(speed), PLAYER_SPEED * Platform_get_frame_time());
	game.players[game.my_id_idx].center = HandleCollision_player_collision(
			game.root,
			game.players, game.players_len,
			game.my_id_idx, speed);

	if (Platform_is_key_pressed(BR_KEY_EQUAL)) {
		game.players[game.my_id_idx].fire_power_up += 1;
	}

	Platform_begin_drawing();
	Platform_clear_background(0xfffbfbfb);
	QuadTree_display(game.root);

	for (u8 i = 0; i < game.players_len; i++) {
		Platform_draw_rectangle(
				game.players[i].center.x-(float)GRID_SIZE,
				game.players[i].center.y-(float)GRID_SIZE,
				GRID_SIZE, GRID_SIZE, 0xff919191);
	}

	for (u8 i = 0; i < game.bombs.size; i++) {
		Platform_draw_rectangle(
				game.bombs.arr[i].bomb_item.center.x-(float)GRID_SIZE,
				game.bombs.arr[i].bomb_item.center.y-(float)GRID_SIZE,
				GRID_SIZE, GRID_SIZE,
				0xff151515);
	}

	for (u8 i = 0; i < game.fires.size; i++) {
		Platform_draw_rectangle(
				game.fires.arr[i].fire_item.center.x-(float)GRID_SIZE,
				game.fires.arr[i].fire_item.center.y-(float)GRID_SIZE,
				GRID_SIZE, GRID_SIZE,
				0xff3030ff);
	}

	Platform_end_drawing();
}

int main()
{
	game = GM_Default_init();
	QuadTree_load_map(game.root, (Block*)level_bytes, size);

	for (u8 i = 0; i < game.players_len; i++) {
		if (game.players[i].id == game.my_id_idx) {
			game.my_id_idx = i;
			break;
		}
	}

#ifndef PLATFORM_WEB
	mm_log();
#endif // PLATFORM_WEB

	Platform_init_window("Bomb royale", SCREEN_WIDTH, SCREEN_HEIGHT);

#ifndef PLATFORM_WEB
	while (!Platform_window_should_close()) {
		game_loop();
	}

	Platform_close_window();
#else
	Platform_set_loop(game_loop);
#endif // PLATFORM_WEB

	return 0;
}
