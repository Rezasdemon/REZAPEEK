#ifndef MEMORY
#define MEMORY

#include <psp2/types.h>
struct memblock {
	uint addr;
	uint size;
};
int GetMemBlocks(uint* count, struct memblock* blocks);
#endif