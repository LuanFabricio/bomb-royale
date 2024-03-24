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
	float half_dimension;
} AABB;

typedef enum {
	AIR,
	DESTRUCTIBLE,
	INDESTRUCTIBLE,
} GridType;

typedef struct {
	GridType grid[GRID_SIZE][GRID_SIZE];
} GameBoard;

typedef struct {
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
	u8 id;
} Player;

typedef struct {
	Point center;
	u8 size, tick_to_explode;
} EphemeralItem;

typedef struct {
	EphemeralItem bomb_item;
} Bomb;

typedef struct {
	EphemeralItem fire_item;
} Fire;

// TODO: Abstract for linux and web
// HACK: Using Raylib key implementation
typedef enum {
	BR_KEY_W = 'W',
	BR_KEY_A = 'A',
	BR_KEY_D = 'D',
	BR_KEY_S = 'S',
	BR_KEY_SPACE = ' ',
} KeysKeyboard;

#ifndef RAYLIB_H
typedef struct {
	float x, y;
} Vector2;
#endif // RAYLIB_H

#endif // TYPE_H_
