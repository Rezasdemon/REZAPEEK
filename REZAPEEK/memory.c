
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <psp2/kernel/sysmem.h>
#include "memory.h"
int GetMemBlocks(uint* count, struct memblock* blocks) {
/*  PRIVATE needs personal implementation
//just a example of methods needed.
	uint start , end;
	start = 0x80000000;
	end = 0x8F000000;

	struct memblock tempblocks[50];

	*count = 0;
	//tempblocks[(*count)].addr = (uint)info.mappedBase;
	tempblocks[(*count)].size = (uint)info.mappedSize;
	
	memcpy(blocks,&tempblocks,4*2*50); 
*/
	return 0;
}
void injectvalue(uint* offset,char* val,int size )
{
	memcpy(offset,val,size);
}
