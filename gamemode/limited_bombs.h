#ifndef GAMEMODE_LIMITED_BOMBS_H
#define GAMEMODE_LIMITED_BOMBS_H

#include "../types.h"
#include "../quadtree.h"
#include "./default.h"

static Game GM_Limited_Bombs_init()
{
	Game game = GM_Default_init();
	game.game_mode = GAMEMODE_LIMITED_BOMBS;
	game.game_mode_details.limited_bombs.remaning_bombs = 3;
	return game;
}

static void GM_Limited_Bombs_on_tick(Game *game, boolean bomb_placed)
{
	if (game->game_mode != GAMEMODE_LIMITED_BOMBS) {
		return;
	}

	if (bomb_placed) {
		if (game->game_mode_details.limited_bombs.remaning_bombs > 0) {
			game->game_mode_details.limited_bombs.remaning_bombs -= 1;
		} else {
			game->players[game->my_id_idx].alive = false;
		}
	}
}

#endif // GAMEMODE_LIMITED_BOMBS_H
