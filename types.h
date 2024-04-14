#ifndef TYPE_H_
#define TYPE_H_

#include "./defines.h"

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef int boolean;
#define false 0
#define true 1

#define NODE_CAPACITY 4

typedef struct {
	float x, y;
} Point;

typedef struct {
	Point center;
	boolean hit_goal;
} CollisionResponse;

typedef struct {
	Point center;
	float half_dimension;
} AABB;

typedef enum {
	AIR,
	DESTRUCTIBLE,
	INDESTRUCTIBLE,
	GOAL,
} GridType;

typedef struct {
	u8 uid;
	Point center;
	GridType grid_type;
} Block;

typedef struct QuadTree QuadTree;
struct QuadTree {
	AABB node;
	u32 objects_size;
	Block objects[NODE_CAPACITY];
	QuadTree *north_west;
	QuadTree *north_east;
	QuadTree *south_west;
	QuadTree *south_east;
};

typedef struct {
	Point center;
	u8 fire_power_up;
	u8 id;
	boolean alive;
} Player;

typedef struct {
	Point center;
	u8 size, tick_to_explode;
} EphemeralItem;

typedef struct {
	EphemeralItem bomb_item;
	u8 fire_power;
} Bomb;

typedef struct {
	Bomb arr[GRID_LENGTH * GRID_LENGTH];
	u8 size;
} BombArray;

typedef struct {
	EphemeralItem fire_item;
} Fire;

typedef struct {
	Fire arr[GRID_LENGTH * GRID_LENGTH];
	u8 size;
} FireArray;

// TODO: Abstract for linux and web
// HACK: Using Raylib key implementation
typedef enum {
	BR_KEY_W = 'W',
	BR_KEY_A = 'A',
	BR_KEY_D = 'D',
	BR_KEY_S = 'S',
	BR_KEY_SPACE = ' ',
	BR_KEY_EQUAL = '=',
} KeysKeyboard;

#ifndef RAYLIB_H
typedef struct {
	float x, y;
} Vector2;
#endif // RAYLIB_H

typedef enum {
	GAMEMODE_DEFAULT,
	GAMEMODE_LIMITED_BOMBS,
} GameMode;

typedef struct {
	QuadTree *root;
	Player players[MAX_PLAYERS];
	u8 my_id, my_id_idx, players_len;
	FireArray fires;
	BombArray bombs;
	u8 bomb_delay;
	boolean hit_goal;
	GameMode game_mode;
	union {
		struct {
			u32 remaning_bombs;
		} limited_bombs;
	} game_mode_details;
} Game;

typedef enum {
	PING,
	UPDATE_MAP,
	UPDATE_PLAYERS,
	REQUEST_MAP,
	REQUEST_PLAYERS,
} GameNetEvent;

typedef struct {
	GameNetEvent event;
	union {
		struct {
			Block data[GRID_LENGTH * GRID_LENGTH];
			u32 size;
		} GameMap;
		struct {
			Player players[4];
		} GamePlayers;
	};
} ServerData;

#endif // TYPE_H_
