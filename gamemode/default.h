#ifndef GAMEMODE_DEFAULT_H_
#define GAMEMODE_DEFAULT_H_

#include "../types.h"
#include "../quadtree.h"

static void GM_Default_init(Game* game)
{
	game->players_len = 1;
	game->players[0].id = 1;
	game->players[0].center.x = GRID_SIZE;
	game->players[0].center.y = GRID_SIZE;
	game->players[0].fire_power_up = 1;
	game->players[0].alive = true;

	game->my_id = 1;
	game->my_id_idx = 0;

	game->root = QuadTree_new((float)HALF_WIDTH, (float)HALF_HEIGHT, (float)HALF_WIDTH);
	game->game_mode = GAMEMODE_DEFAULT;
}

#endif // GAMEMODE_DEFAULT_H_
