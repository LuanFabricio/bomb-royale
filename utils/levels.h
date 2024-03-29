#ifndef LEVELS_H_
#define LEVELS_H_

#include "../types.h"
#include <stdio.h>
#include <string.h>

#define LEVELS static

LEVELS void save_grid_as_bin(const char* filepath, const Block *grid, const u32 *blocks_size)
{
	FILE *file = fopen(filepath, "wb");

	fwrite(grid, sizeof(Block), GRID_LENGTH*GRID_LENGTH, file);
	fwrite(blocks_size, sizeof(u32), 1, file);

	fclose(file);
}

LEVELS void save_grid_as_header(const char* filepath, const u8 *grid, const u32 blocks_size)
{
	FILE *file = fopen(filepath, "w");

	fprintf(file, "#ifndef LEVEL_H_\n#define LEVEL_H_\n");
	fprintf(file, "static unsigned char level_bytes[] = { ");
	for (size_t i = 0; i < sizeof(Block)*blocks_size; i++) {
		fprintf(file, "0x%x,", grid[i]);
	}
	fprintf(file, "};\n");
	fprintf(file, "static unsigned int size = %u;\n", blocks_size);
	fprintf(file, "#endif // LEVEL_H_");

	fclose(file);
}

LEVELS void load_grid_by_bin(const char* filepath, Block *grid, u32 *blocks_size)
{
	FILE *file = fopen(filepath, "rb");
	if (file == NULL) {
		return;
	}

	fread(grid, sizeof(Block), GRID_LENGTH*GRID_LENGTH, file);
	fread(blocks_size, sizeof(u32), 1, file);

	fclose(file);
}

#endif // LEVELS_H_
