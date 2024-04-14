#ifndef GAMEMODE_HANDLER_H_
#define GAMEMODE_HANDLER_H_

#include "../types.h"
#include "./limited_bombs.h"

static boolean GM_Handler_should_place_bomb(const Game* game)
{
	switch (game->game_mode) {
		case GAMEMODE_LIMITED_BOMBS:
			return GM_Limited_Bombs_should_place_bomb(game);
		default:
			return true;
	}
}

static void GH_Handler_on_tick(Game* game, boolean bomb_placed)
{

	switch (game->game_mode) {
		case GAMEMODE_LIMITED_BOMBS:
			GM_Limited_Bombs_on_tick(game, bomb_placed);
			break;
		default:
			break;
	}
}

#endif // GAMEMODE_HANDLER_H_
