

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <string.h>
#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/appmgr.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/display.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/processmgr.h>

#include <taihen.h>

#include "blit.h"
#include "threads.h"
#include "ui.h"

int started = 0;
int ram_mode = 0;
SceCtrlData pad, oldpad;
int runtype = 0;



void MakeDIRS(int type);

int main_thread(SceSize args, void *argp) {
	
	sceKernelDelayThread(5 * 1000 * 1000);
	MakeDIRS(runtype);
	
	
	// Attaching game main thread
	SceKernelThreadInfo status;
	status.size = sizeof(SceKernelThreadInfo);
	sceKernelGetThreadInfo(0x40010003, &status);
	
	// Check if we'll use RAM or MMC storage
	//SceCtrlData pad, oldpad;
	sceCtrlPeekBufferPositive(0, &oldpad, 1);
	if (!(oldpad.buttons & SCE_CTRL_LTRIGGER)){
		uint8_t* test = malloc(status.stackSize);
		if (test != NULL){
			ram_mode = 1;
			free(test);
		}
	}
	// Getting title info
	//char titleid[16], title[256];
	sceAppMgrAppParamGetString(0, 9, title , 256);
	sceAppMgrAppParamGetString(0, 12, titleid , 256);
	
	//initialze and concat menu names with [] around current menu for top bar;
	imenu = 0;
	imenu_opt = 0;
		
	menusbuf = malloc(60);
	memset(menusbuf , 0 , 60);
	updatemenusbuf(0);
	
	// Main loop
	for (;;){
		
		sceCtrlPeekBufferPositive(0, &pad, 1);
		if (started){
			displayMenu(ram_mode,pad,oldpad);
			if ((pad.buttons & SCE_CTRL_LTRIGGER) && (!(oldpad.buttons & SCE_CTRL_LTRIGGER))){
				if (imenu > 0){
					imenu--;
					imenu_opt = 0;
					updatemenusbuf(imenu);
					blit_clearscreen();
				}
			}
			if ((pad.buttons & SCE_CTRL_RTRIGGER) && (!(oldpad.buttons & SCE_CTRL_RTRIGGER))){
				if (imenu < 4){
					imenu++;
					imenu_opt = 0;
					updatemenusbuf(imenu);
					blit_clearscreen();
				}
			}
			
			if ((pad.buttons & SCE_CTRL_START) && (!(oldpad.buttons & SCE_CTRL_START))){
				started = 0;
				resumeMainThread();
			}
		}
		else
		{
			if ((pad.buttons & SCE_CTRL_SELECT) && (pad.buttons & SCE_CTRL_START)){
				pauseMainThread();
				started = 1;
				blit_clearscreen();
			}
			else
			{
				sceKernelDelayThread(1000);// Invoking scheduler to not slowdown games
			}
		}
		oldpad = pad;
	}
	free(menusbuf);
	
	return 0;
}


int _patchrun(SceSize args,void *argp){
	runtype = 1;
	main_thread(args,argp);
	return 0;
}
int _start(SceSize args,const void *argp) {
	
	SceUID thid = sceKernelCreateThread("REZAPEEK", _patchrun, 0x40, 0x600000, 0, 0, NULL);
	if (thid >= 0)
		sceKernelStartThread(thid, 0, NULL);
	return 0;
}

// our own plugin entry
int _taihen(SceSize args,void *argp){
	runtype = 2;
	main_thread(args,argp);
	return 0;
}
int module_start(SceSize argc, const void *args) {
	//sceKernelExitProcess(0);
	SceUID thid = sceKernelCreateThread("REZAPEEK", _taihen, 0x40, 0x600000, 0, 0, NULL);
	if (thid >= 0)
		sceKernelStartThread(thid, 0, NULL);
	
	return 0;
}

int module_stop(SceSize argc, const void *args) {
  return SCE_KERNEL_STOP_SUCCESS;
}

/**
 * @brief      Module Exit handler (unused)
 *
 *             This function is currently unused on retail units.
 */
void module_exit(void) {

}

void MakeDIRS(int type){
	switch(type){
		case 1:
		sceIoMkdir("ux0:/data/REZAPEEK", 0777);
		sceIoMkdir("ux0:/data/REZAPEEK/db", 0777);
		sceIoMkdir("ux0:/data/REZAPEEK/tmp", 0777);
		break;
		case 2:
		sceIoMkdirForDriver("ux0:/data/REZAPEEK", 0777);
		sceIoMkdirForDriver("ux0:/data/REZAPEEK/db", 0777);
		sceIoMkdirForDriver("ux0:/data/REZAPEEK/tmp", 0777);
		break;
	}
	
}

