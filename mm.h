#ifndef MM_H_
#define MM_H_

#include "./defines.h"
#include "./types.h"

#define MEMORY_SIZE 1024 * 16

static u8 memory[MEMORY_SIZE] = {0};
static u32 memory_ptr = 0;

static void* mm_malloc(u64 size)
{
	if (size < MEMORY_SIZE && memory_ptr + size < MEMORY_SIZE) {
		memory_ptr += size;
		return (void*)&memory[memory_ptr];
	}

	return NULL;
}

static void mm_clean()
{
	memory_ptr = 0;
}

#ifndef PLATFORM_WEB
#include <stdio.h>
static void mm_log()
{
	float p = memory_ptr * 100;
	p /= (float)MEMORY_SIZE;
	printf("Memory: %u/%u(%.02f%%)\n", memory_ptr, MEMORY_SIZE, p);
}
#endif

#endif // MM_H_
