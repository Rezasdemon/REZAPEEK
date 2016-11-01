/*
VOID foo(int* pnA, int& nB)
{
  *pnA = 1;
  nB = 2;
}

int nA, nB;
foo(&nA, nB);
// nA == 1, nB == 2;
*/
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


#include "blit.h"
#include "math.h"
#include "ui.h"


//##################### Declarations #####################//

int clr_highlight = 0x0000ffff;
int clr_normal = 0x00ffffff;

char* getMenuName(int id);

void line();
int _curline = 0;
int curline();
void updownmenuopt(SceCtrlData pad, SceCtrlData oldpad, int options);
void controls_intleftright(int* i ,int min  ,int max , SceCtrlData pad, SceCtrlData oldpad);

void populatemenu(struct menuoptions menuopt[] , int ioptions);

void menu_main();
void menu_search();
void menu_database();
void menu_viewmem();
void menu_info();

//##################### ENTRY POINTS/MAIN FUNC #####################//


void displayMenu(int ram_mode,SceCtrlData pad, SceCtrlData oldpad){
	
	
	sceDisplayWaitVblankStart();
	blit_setup();
	_curline=5;
	char* MenuName = getMenuName(imenu);
	blit_stringf(5, 5, "REZAPEEK v.0.1 - Menu -> %s", MenuName);
	
	blit_stringf(5, curline(), "%s", menusbuf);
	line(curline());
	
	enum CMenu eMenu = imenu;
	
	switch(eMenu)
	{
		case MAIN:
		menu_main(pad,oldpad);
		break;
		case SEARCH:
		menu_search(pad , oldpad);
		break;
		case VIEWMEM:
		menu_viewmem(pad , oldpad);
		break;
		case DATABASE:
		menu_database(pad,oldpad);
		break;
		case INFO:
		menu_info(pad,oldpad);
		break;
		
		default:
		break;
	}
	
	line(480);
	blit_stringf(5, 500, "Title: %s", title);
	blit_stringf(5, 520, "TitleID: %s", titleid);
	blit_stringf(830, 520, ram_mode ? "RAM Mode" : "MMC Mode");
}

//##################### FUNCTIONS #####################//

//enum menuid{main,search,viewmem,database,info};
char* getMenuName(int id){
	enum CMenu eMenu = id;
	switch(eMenu)
	{
		case MAIN:
		return (char*)"MAIN";
		break;
		case SEARCH:
		return (char*)"SCAN";
		break;
		case VIEWMEM:
		return (char*)"PEEKMEM";
		break;
		case DATABASE:
		return (char*)"DB";
		break;
		case INFO:
		return (char*)"INFO";
		break;
		default:
		return (char*)"Undefined";
		break;
		
		
	}
	return (char*)"Undefined";
}

void line(int y){
	blit_stringf(5, y, "____________________________________________________________", "");
}

int curline (){
	_curline +=20;
	return _curline;
}



//fills the menus buffer with all the menu names and a [] around the current menu
//displays at top
void updatemenusbuf(int id){
	memset(menusbuf,0,60);
	int i = 0;
	int y = 0;
	while (i<60)
	{
		if(y<5){
			if(i + 1 < 60){
				if(id == y){
					memcpy(menusbuf + i , "[",1);
					i+= 1;
				}
			}
			char* name = getMenuName(y);
			if(i+strlen(name) < 60){
			memcpy(menusbuf+i , name,strlen(name));
			i+= strlen(name);
			}
				if(i + 1 < 60){}
			if(id == y){
			memcpy(menusbuf + i , "]",1);
			i+= 1;
			}
				if(i + 1 < 60){}
			if(y < 4){
			memcpy(menusbuf + i , " - ",3);
			i+= 3;
			}
			y++;
		}
		else
		{i++;}
	}
}

//handle up and down for menu
void updownmenuopt(SceCtrlData pad , SceCtrlData oldpad, int ioptions){
if ((pad.buttons & SCE_CTRL_UP) && (!(oldpad.buttons & SCE_CTRL_UP))){
				if (imenu_opt > 0){
					imenu_opt--;
				}
			}
			if ((pad.buttons & SCE_CTRL_DOWN) && (!(oldpad.buttons & SCE_CTRL_DOWN))){
				if (imenu_opt < ioptions-1){
					imenu_opt++;
				}
			}

}

void controls_intleftright(int* i ,int min  ,int max , SceCtrlData pad, SceCtrlData oldpad){
	

	if ((pad.buttons & SCE_CTRL_LEFT) && (!(oldpad.buttons & SCE_CTRL_LEFT)))
	{
		if((uint)*i > (uint)min){
			*i = *i - 1;
		}
	}
	if ((pad.buttons & SCE_CTRL_RIGHT) && (!(oldpad.buttons & SCE_CTRL_RIGHT)))
	{
		if ((uint)*i<(uint)max){
			*i = *i +1;			
		}
	}	

}


//dynamically display menu from struct based on current selection
void populatemenu(struct menuoptions menuopt[], int ioptions)
{
	int i;
	for(i=0;i<ioptions;i++){
		if(i==imenu_opt){
			
			blit_set_color(clr_highlight);
		}
		
		blit_stringf(5, curline(), menuopt[i].left, menuopt[i].right);
		
		if(i == imenu_opt)
		{
			blit_set_color(clr_normal);
		}
	}
}


//##################### MENUS #####################//

int val = 0;
int ihextest = 0x80000000;

//menu code to hande menus 
//enum menuid{main,search,viewmem,database,info};
void menu_main(SceCtrlData pad, SceCtrlData oldpad)
{
	int ioptions = 4;
	
	struct menuoptions menuopt[ioptions];
	
	//first menu option
	char opt1[10];
	menuopt[0].left = "cur MenuValue: %s";
	itoa(val,opt1,10);
	menuopt[0].right = opt1;
	if(imenu_opt == 0){
		controls_intleftright(&val,0,10,pad,oldpad);
	}
	
    //second menu option
	menuopt[1].left = "Hex int To String: %s";
	char opt2 [17];
	
	snprintf(opt2, 17, "0x%08X",ihextest);
	//itoa(ihextest , opt2,16);
	menuopt[1].right = opt2;
	if (imenu_opt == 1){
		controls_intleftright(&ihextest, 0 , 0xffffffff, pad ,oldpad);	
	}
	
	
	//third menu option
	char opt3[10];
	itoa(getstrlength(opt2,10),opt3,10);
	menuopt[2].left = "Buffer len =  %s";
	menuopt[2].right = opt3;
	
	//fourth
	char opt4[10];
	itoa(imenu_opt,opt4,10);
	menuopt[3].left ="menuopt : %s";
	menuopt[3].right= opt4;
	
	/*
	
	if ((pad.buttons & SCE_CTRL_LEFT) && (!(oldpad.buttons & SCE_CTRL_LEFT)))
	{
		switch(imenu_opt){
		case 0:
		if (val > 0){
		val--;
		}
		break;
		case 1:
		if(ihextest >0x00000000){
			ihextest --;
		}
		break;
		
		}
	}
	if ((pad.buttons & SCE_CTRL_RIGHT) && (!(oldpad.buttons & SCE_CTRL_RIGHT)))
	{
		switch(imenu_opt){
		case 0:
		if (val < 5){
		val++;
		}
		break;
		case 1:
		if(ihextest <0xFFFFFFFF){
			ihextest ++;
		}
		break;
		}
	}
	*/
	
	
	updownmenuopt(pad ,oldpad,ioptions);
	
	populatemenu(menuopt, ioptions);
	
	
}

void menu_search(SceCtrlData pad, SceCtrlData oldpad)
{
	int ioptions = 1;
	//char _Buf[2 * 9];
	
	struct menuoptions menuopt[ioptions];
	
	
	menuopt[0].left = "TODO: %s menu";
	menuopt[0].right= getMenuName(imenu);
	
	
	updownmenuopt(pad ,oldpad,ioptions);
	
	populatemenu(menuopt, ioptions);
	
}

void menu_viewmem(SceCtrlData pad, SceCtrlData oldpad)
{

	int ioptions = 1;
	//char _Buf[2 * 9];
	
	struct menuoptions menuopt[ioptions];
	
	menuopt[0].left = "TODO: %s menu";
	menuopt[0].right= getMenuName(imenu);
	
	updownmenuopt(pad ,oldpad,ioptions);
	
	populatemenu(menuopt, ioptions);
	
}

void menu_database(SceCtrlData pad, SceCtrlData oldpad)
{
	int ioptions = 3;
	//char _Buf[2 * 9];
	
	struct menuoptions menuopt[ioptions];
	
	
	menuopt[0].left = "TODO: %s menu";
	menuopt[0].right= getMenuName(imenu);
	
	menuopt[1].left = "some generic text to test option selection";
	menuopt[1].right="";
	
	menuopt[2].left = "more generic text";
	menuopt[2].right= "";
	
	
	updownmenuopt(pad ,oldpad,ioptions);
	
	populatemenu(menuopt, ioptions);
	
}

void menu_info(SceCtrlData pad, SceCtrlData oldpad)
{
	blit_stringf(5, curline(), "%s", "infoScreen");
	blit_stringf(5, curline(), "%s", "Written by: Reza");
	blit_stringf(5, curline(), "%s", "credits to : OneRice07 , Rinnegatamante");
	
}



