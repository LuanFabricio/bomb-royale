#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include "../types.h"
#include "../quadtree.h"

typedef enum { X, Y } Axis;

static void _HandleCollision_move_player_aabb(AABB *aabb1, float position, Vector2 speed, Axis axis)
{
	switch (axis) {
		case X:
			if (speed.x > 0) aabb1->center.x = position - GRID_SIZE;
			else if (speed.x < 0) aabb1->center.x = position + GRID_SIZE;
			break;
		case Y:
			if (speed.y > 0) aabb1->center.y = position - GRID_SIZE;
			else if (speed.y < 0) aabb1->center.y = position + GRID_SIZE;
			break;
	}
}

// static AABB HandleCollision_aabb_quadtree(QuadTree* root, AABB aabb, Vector2 speed)
// {
// 	aabb.center.x += speed.x;
// 	Block *collision = NULL;
// 	if ((collision = QuadTree_check_collision(root, aabb))) {
// 		// printf("Collision: %p\n", collision);
// 		// printf("Collision: %.2f %.2f\n", collision.center.x, collision->center.y);
//
// 		_HandleCollision_move_player_aabb(&aabb, collision->center.x, speed, X);
// 	}
//
// 	aabb.center.y += speed.y;
// 	collision = NULL;
// 	if ((collision = QuadTree_check_collision(root, aabb))) {
// 		// printf("Collision: %p\n", collision);
// 		// printf("Collision: %.2f %.2f\n", collision.center.x, collision->center.y);
//
// 		_HandleCollision_move_player_aabb(&aabb, collision->center.y, speed, Y);
// 	}
//
// 	return aabb;
// }

// static AABB HandleCollision_players(const Player *players, const u8 players_len, u8 player_idx, Vector2 speed)
// {
// 	AABB player_aabb = {
// 		.center = players[player_idx].center,
// 		.half_dimension = GRID_SIZE,
// 	};
// 	player_aabb.center.x += speed.x;
//
// 	u8 i;
//
// 	player_aabb.center.y += speed.y;
// 	for (i = 0; i < players_len; i++) {
// 		if (i == player_idx) continue;
//
// 		if (AABB_contains_point(player_aabb, players[i].center)) {
// 			_HandleCollision_move_player_aabb(&player_aabb, players[i].center.y, speed, Y);
// 			break;
// 		}
// 	}
//
// 	return player_aabb;
// }

static void _HandleCollision_obj_collider(const QuadTree* root, AABB* player_aabb, Vector2 speed, Axis axis)
{
	Block *collision = NULL;
	if ((collision = QuadTree_check_collision(root, *player_aabb))) {
		// printf("Collision: %p\n", collision);
		// printf("Collision: %.2f %.2f\n", collision.center.x, collision->center.y);

		float value = 0;
		switch (axis) {
			case X:
				value = collision->center.x;
				break;
			case Y:
				value = collision->center.y;
				break;
		}
		_HandleCollision_move_player_aabb(player_aabb, value, speed, axis);
	}
}

static void _HandleCollision_player_collider(const Player* players, const u8 players_len, u8 player_idx, AABB *player_aabb, Vector2 speed, Axis axis)
{
	for (u8 i = 0; i < players_len; i++) {
		if (i == player_idx) continue;

		if (AABB_contains_point(*player_aabb, players[i].center)) {
			float value = 0;
			switch (axis) {
				case X:
					value = players[i].center.x;
					break;
				case Y:
					value = players[i].center.y;
					break;
			}
			_HandleCollision_move_player_aabb(player_aabb, value, speed, axis);
			break;
		}
	}
}

static Point HandleCollision_player_collision(const QuadTree* root, const Player* players, u8 players_len, u8 player_idx, Vector2 speed)
{
	AABB player_aabb = {
		.center = players[player_idx].center,
		.half_dimension = GRID_SIZE - 1,
	};

	player_aabb.center.x += speed.x;
	_HandleCollision_obj_collider(root, &player_aabb, speed, X);
	_HandleCollision_player_collider(players, players_len, player_idx, &player_aabb, speed, X);


	player_aabb.center.y += speed.y;
	_HandleCollision_obj_collider(root, &player_aabb, speed, Y);
	_HandleCollision_player_collider(players, players_len, player_idx, &player_aabb, speed, Y);

	return player_aabb.center;
}

#endif // COLLISION_HANDLER_H
