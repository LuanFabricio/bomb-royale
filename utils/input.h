#ifndef INPUT_H_
#define INPUT_H_

#include "../types.h"
#include "../platform.h"
#include "../defines.h"
#include "./math.h"

static Vector2 Input_speed()
{
	Vector2 speed = {0};

	if (Platform_is_key_down(BR_KEY_D)) {
		speed.x = 1;
	} else if (Platform_is_key_down(BR_KEY_A)) {
		speed.x = -1;
	} else {
		speed.x = 0;
	}

	if (Platform_is_key_down(BR_KEY_W)) {
		speed.y = -1;
	} else if (Platform_is_key_down(BR_KEY_S)) {
		speed.y = 1;
	} else {
		speed.y = 0;
	}

	return speed;
}

static boolean Input_place_bomb(BombArray *bombs, Player players[], u8* bomb_delay, u8 my_id_idx)
{
	if (*bomb_delay == 0 && Platform_is_key_down(BR_KEY_SPACE)) {
		bombs->arr[bombs->size] = (Bomb) {
			.bomb_item = {
				.center = players[my_id_idx].center,
				.size = 3,
				.tick_to_explode = BOMB_NORMAL_TICKS
			},
				.fire_power = minu8(players[my_id_idx].fire_power_up, GRID_LENGTH),
		};
		bombs->arr[bombs->size].bomb_item.center.x = (int)(players[my_id_idx].center.x / GRID_SIZE);
		bombs->arr[bombs->size].bomb_item.center.x *= GRID_SIZE;

		bombs->arr[bombs->size].bomb_item.center.y = (int)(players[my_id_idx].center.y / GRID_SIZE);
		bombs->arr[bombs->size].bomb_item.center.y *= GRID_SIZE;

		bombs->size++;
		*bomb_delay = BOMB_DELAY_TICK;

		return true;
	} else if (*bomb_delay > 0) {
		*bomb_delay -= 1;
	}

	return false;
}

#endif // INPUT_H_
