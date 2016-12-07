
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <psp2/kernel/sysmem.h>
#include "memory.h"

int GetMemBlocks(uint* count, struct memblock* blocks) {
	uint start , end;
	start = 0x80000000;
	end = 0x8F000000;
	if (start < 0x60000000 || start > 0xF0000000 || end < 0x60000000 || end > 0xF0000000)
		return -1;
	
	struct memblock tempblocks[50];
	

	uint32_t i = start;
	*count = 0;
	while (i < end) {
		SceKernelMemBlockInfo info;
		memset(&info, 0, sizeof(SceKernelMemBlockInfo));
		info.size = sizeof(SceKernelMemBlockInfo);
		if (sceKernelGetMemBlockInfoByRange((void *)i, 0x1000, &info) >= 0) {
			//SceUID blockid = sceKernelFindMemBlockByAddr(info.mappedBase, 0); // fails on module executable blocks
			//debugPrintf("0x%08X, 0x%08X, 0x%08X: 0x%08X\n", info.mappedBase, info.mappedSize, info.type, blockid);
			i = (uint32_t)info.mappedBase + info.mappedSize;
			if(info.mappedSize >200*1024){
		tempblocks[(*count)].addr = (uint)info.mappedBase;
		tempblocks[(*count)].size = (uint)info.mappedSize;
		(*count)++;
		}
		} else {
			
			i += 0x1000;
		}
		
	}
	memcpy(blocks,&tempblocks,4*2*50); 
	//debugPrintf("Found %d memory blocks\n", count);

	return 0;
}
void injectvalue(uint* offset,char* val,int size )
{
	memcpy(offset,val,size);
}
