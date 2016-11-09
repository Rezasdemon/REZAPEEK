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

#include <psp2/power.h>
#include <psp2/io/stat.h>

#include <psp2/appmgr.h>
#include <psp2/apputil.h>
#include <psp2/audioout.h>
#include <psp2/ctrl.h>
#include <psp2/display.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/ime_dialog.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/message_dialog.h>

#include <psp2/motion.h>
#include <psp2/pgf.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <psp2/sysmodule.h>
#include <psp2/system_param.h>
#include <psp2/touch.h>
#include <psp2/types.h>

#include "memory.h"
#include "blit.h"
#include "math.h"
#include "ui.h"


//##################### Declarations #####################//

int clr_highlight = 0x0000ffff;
int clr_normal = 0x00ffffff;

int hxplace = 0;
int hxincrement = 1;


void injectvalue(uint* offset,char* val,int size );


char* getMenuName(int id);

void line();
int pressed = 0;
int redraw = 1;
int _curline = 0;
int curline();

void control_tip(char* tip);
void controls_crosssetint(uint* offset, char* val , int size,SceCtrlData pad , SceCtrlData oldpad);
void updownmenuopt(SceCtrlData pad, SceCtrlData oldpad, int options);
void controls_intleftright(uint* i ,int min  ,int max , SceCtrlData pad, SceCtrlData oldpad);
void controls_hexleftright(uint* i,int size , SceCtrlData pad, SceCtrlData oldpad);
void controls_selfindmem(uint* count,struct memblock* blocks,SceCtrlData pad, SceCtrlData oldpad);


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
	if(pressed == 1)
	{
		pressed = 0;
		blit_clearscreen();
	}
	if(pressed==2)
	{
		pressed = 0;
		blit_stringf(960/2, 500, "Set Memory.");
	}
	_curline=5;
	char* MenuName = getMenuName(imenu);
	blit_stringf(5, 5, "REZAPEEK v.0.2 - Menu -> %s", MenuName);
	
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
void injectvalue(uint* offset,char* val,int size )
{
	memcpy(offset,val,size);
}

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
	if(id == VIEWMEM)
	{
		redraw = 1;
	}
}



//tip to display on current option
void control_tip(char* tip){
	blit_stringf(960/2-50, 520, tip);
	
}
// with hex input for address and int on cross pressed inject value to address
void controls_crosssetint(uint* offset, char* val , int size,SceCtrlData pad,SceCtrlData oldpad){
	if ((pad.buttons & SCE_CTRL_CROSS) && (!(oldpad.buttons & SCE_CTRL_CROSS))){
				
				injectvalue(offset,val,size);
				pressed = 2;	
				}
	
}

//handle up and down for menu changes imenu_opt;
void updownmenuopt(SceCtrlData pad , SceCtrlData oldpad, int ioptions){
if ((pad.buttons & SCE_CTRL_UP) && (!(oldpad.buttons & SCE_CTRL_UP))){
				if (imenu_opt > 0){
					imenu_opt--;
				}
				pressed = 1;	
				hxplace = 0;
			}
			if ((pad.buttons & SCE_CTRL_DOWN) && (!(oldpad.buttons & SCE_CTRL_DOWN))){
				if (imenu_opt < ioptions-1){
					imenu_opt++;
				}
				pressed = 1;
				hxplace = 0;
				
			}
}

// add controls for int input editing
void controls_intleftright(uint* i ,int min  ,int max , SceCtrlData pad, SceCtrlData oldpad){
	

	if ((pad.buttons & SCE_CTRL_LEFT) && (!(oldpad.buttons & SCE_CTRL_LEFT)))
	{
		if((uint)*i > (uint)min){
			*i = *i - 1;
		}
		pressed = 1;
	}
	if ((pad.buttons & SCE_CTRL_RIGHT) && (!(oldpad.buttons & SCE_CTRL_RIGHT)))
	{
		if ((uint)*i<(uint)max){
			*i = *i +1;			
		}
		pressed = 1;
	}	

}

// add controls for hex input editing 
void controls_hexleftright(uint* i,int size , SceCtrlData pad, SceCtrlData oldpad){
	
	if ((pad.buttons & SCE_CTRL_LEFT) && (!(oldpad.buttons & SCE_CTRL_LEFT)))
	{
		if(hxplace < size*2 -1 ){
			hxplace ++;
		}
		pressed = 1;
		
	}
	if ((pad.buttons & SCE_CTRL_RIGHT) && (!(oldpad.buttons & SCE_CTRL_RIGHT)))
	{
		if(hxplace> 0){
			hxplace--;
		}
		pressed = 1;
		
	}	
	if((pad.buttons & SCE_CTRL_SQUARE) && (!(oldpad.buttons & SCE_CTRL_SQUARE))){
		hxincrement = 0x1 << hxplace * 4 ;
		uint curval = (uint)(*i & (0xf << hxplace * 4))>>hxplace*4;
		if(curval> 0x00) {
			*i -= hxincrement;
		}
		if(curval == 0x00){
			hxincrement = 0xF << hxplace * 4 ;
			*i+= hxincrement;
		}
		pressed = 1;
	}
	if((pad.buttons & SCE_CTRL_TRIANGLE) && (!(oldpad.buttons & SCE_CTRL_TRIANGLE))){
		
		hxincrement = 0x1 << hxplace * 4 ;
		uint curval = (uint)(*i & (0xf << hxplace * 4))>>hxplace*4;
		if(curval< 0x0F) {
			*i += hxincrement;
		}
		if(curval == 0x0F)
		{	hxincrement = 0xF << hxplace * 4 ;
		*i -= hxincrement;
		}
		pressed = 1;			
	}
	char buffer[10];
	itoa(hxplace+1,buffer,10);
	blit_stringf(830, 500, "Place: %s", buffer);
}

void controls_selfindmem(uint* count,struct memblock* blocks, SceCtrlData pad, SceCtrlData oldpad){
	

	if ((pad.buttons & SCE_CTRL_SELECT) && (!(oldpad.buttons & SCE_CTRL_SELECT)))
	{
			GetMemBlocks(count,blocks);
		}
	

}





//dynamically display menu from struct based on current selection
void populatemenu(struct menuoptions menuopt[],int ioptions)
{
	int i;
	
	for(i=0;i<ioptions;i++){
		if(i==imenu_opt){
			
			blit_set_color(clr_highlight);
		}
		int x =menuopt[i].useline == 1 ? menuopt[i].x: 5;
		int y =menuopt[i].useline == 1 ? menuopt[i].y: curline();
		blit_stringf(x, y, menuopt[i].left, menuopt[i].right);
		
		if(i == imenu_opt)
		{
			blit_set_color(clr_normal);
		}
	}
}

//##################### MENUS #####################//

uint sval = 0xffff;
uint iaddress = 0x816FB16C;
uint memblocks = 0;
struct memblock memaddrs[50];
void menu_main(SceCtrlData pad, SceCtrlData oldpad)
{
	int ioptions = 4;
	
	struct menuoptions menuopt[ioptions];
	
    //SET ADDRESS 
	menuopt[0].left = "ADDRESS: %s";
	char straddress[11];
	hexinttostring(iaddress,gettypesize(t_int)*2,straddress);
	menuopt[0].right = straddress;
	menuopt[0].useline = 0;
	if (imenu_opt == 0){
		controls_hexleftright(&iaddress,gettypesize(t_int), pad ,oldpad);
	}
	
	//SET VALUE
	menuopt[1].left = "VALUE: %s";
	char strsetto[11];
	hexinttostring(sval,gettypesize(t_short)*2,strsetto);// 2bytes has 4 places in hex
	menuopt[1].right = strsetto;
	menuopt[1].useline = 0;
	if (imenu_opt == 1){
		controls_hexleftright(&sval,gettypesize(t_short), pad ,oldpad);	 // pass size of 2 bytes 
		sval = swap_uint16(sval);
		controls_crosssetint( (uint*)iaddress, (char*)&sval , gettypesize(t_short),pad,oldpad);
		sval = swap_uint16(sval);
		control_tip("X=Inject");
	}
	
	menuopt[2].left = "MEMBLOCKS = %s";
	char buffer[10];
	itoa(memblocks,buffer,10);
	menuopt[2].right = buffer;
	menuopt[2].useline = 0;
	if(imenu_opt == 2)
	{
		controls_selfindmem(&memblocks,(struct memblock*)(&memaddrs),pad,oldpad);
	}
	
	
	menuopt[3].left = "MEMBLOCKS loc = %s";
	menuopt[3].left = "ADDRESS: %s";
	char lbuffer[11];
	hexinttostring((int)&memaddrs,gettypesize(t_int)*2,lbuffer);
	menuopt[3].right = lbuffer;
	menuopt[3].useline = 0;
	
	
	
	updownmenuopt(pad ,oldpad,ioptions);
	
	populatemenu(menuopt, ioptions);
	curline();	
	
	
}

uint hextest2 = 0x000f0000;
void menu_search(SceCtrlData pad, SceCtrlData oldpad)
{
	int ioptions = 2;
	//char _Buf[2 * 9];
	
	struct menuoptions menuopt[ioptions];
	
	
	menuopt[0].left = "TODO: %s menu";
	menuopt[0].right= getMenuName(imenu);
	menuopt[0].useline = 0;
	
	menuopt[1].left = "Hex Test2 : %s";
	char strhextest2[11];
	hexinttostring(hextest2,gettypesize(t_int)*2,strhextest2);
	menuopt[1].right = strhextest2;
	menuopt[1].useline = 0;
	if (imenu_opt == 1){
		controls_hexleftright(&hextest2,gettypesize(t_int), pad ,oldpad);	
	}
	
	updownmenuopt(pad ,oldpad,ioptions);
	
	populatemenu(menuopt, ioptions);
	
}



uint vmsval = 0xffff;
uint vmiaddress = 0x816FB16C;

void menu_viewmem(SceCtrlData pad, SceCtrlData oldpad)
{
int ioptions = 2;
	
	struct menuoptions menuopt[ioptions];
	
	int controlheight = curline();
	
    //SET ADDRESS 
	menuopt[0].left = "ADDRESS: %s";
	char straddress[11];
	hexinttostring(vmiaddress,gettypesize(t_int)*2,straddress);
	menuopt[0].right = straddress;
	menuopt[0].useline = 1;
	menuopt[0].x=5;
	menuopt[0].y=controlheight;
	
	
	if (imenu_opt == 0){
		controls_hexleftright(&vmiaddress,gettypesize(t_int), pad ,oldpad);
	}
	
	//SET VALUE
	menuopt[1].left = "VALUE: %s";
	char strsetto[11];
	hexinttostring(vmsval,gettypesize(t_short)*2,strsetto);// 2bytes has 4 places in hex
	menuopt[1].right = strsetto;
	menuopt[1].useline = 1;
	menuopt[1].x=325;
	menuopt[1].y=controlheight;
	
	if (imenu_opt == 1){
		controls_hexleftright(&vmsval,gettypesize(t_short), pad ,oldpad);	 // pass size of 2 bytes 
		vmsval = swap_uint16(vmsval);
		controls_crosssetint( (uint*)vmiaddress, (char*)&vmsval , gettypesize(t_short),pad,oldpad);
		vmsval = swap_uint16(vmsval);
		control_tip("X=Inject");
	}
	
	updownmenuopt(pad ,oldpad,ioptions);
	
	populatemenu(menuopt, ioptions);
	
	if(redraw == 1){
	line(curline());
	
		redraw = 0;
		int x,y;
		int height = 0;
		int xoffset = 30;
		int cellwidth = 45;
		int adroffset =0;
		char memoutput[2];
		
		for(y = 0 ; y < 15;y++)
		{
			
			height = curline(); 
			
			if(y==0){
				
				for(x=0;x<16;x++){
				int header = vmiaddress+x;
				int value = 0;
				memcpy(&value,&header,1);
				hexinttostringnoleading(value,gettypesize(t_byte)*2,memoutput);// 2bytes has 4 places in hex
				blit_stringf(190+(x*cellwidth)+xoffset,height,"%s", memoutput);
			
				}
				line(curline());
			height = curline();
			}
			
			blit_stringf(5,height,"%s","|");
			blit_stringf(170+xoffset,height,"%s","/");
			blit_stringf(960-20,height,"%s","|");
			
			
			int lineaddr = vmiaddress+ adroffset;
			adroffset += (0x1<<1*4);
			char slineaddr[11];
			hexinttostring(lineaddr,gettypesize(t_int)*2,slineaddr);
			blit_stringf(xoffset,height,"%s", slineaddr);
			
			for(x=0;x<16;x++){
				uint* addressvalue = (uint*)(vmiaddress+(y*16)+x);
				int value = 0;
				memcpy(&value,addressvalue,1);
				hexinttostringnoleading(value,gettypesize(t_byte)*2,memoutput);// 2bytes has 4 places in hex
				blit_stringf(190+(x*cellwidth)+xoffset,height,"%s", memoutput);
			}
			
		}
	line(curline());	
	}
	
	if(pressed ==1 ){
		redraw = 1;
	}
	
}

void menu_database(SceCtrlData pad, SceCtrlData oldpad)
{
	int ioptions = 3;
	//char _Buf[2 * 9];
	
	struct menuoptions menuopt[ioptions];
	
	
	menuopt[0].left = "TODO: %s menu";
	menuopt[0].right= getMenuName(imenu);
	menuopt[0].useline = 0;
	
	menuopt[1].left = "some generic text to test option selection";
	menuopt[1].right="";
	menuopt[1].useline = 0;
	
	menuopt[2].left = "more generic text";
	menuopt[2].right= "";
	menuopt[2].useline = 0;
	
	
	updownmenuopt(pad ,oldpad,ioptions);
	
	populatemenu(menuopt, ioptions);
	
}

void menu_info(SceCtrlData pad, SceCtrlData oldpad)
{
	blit_stringf(5, curline(), "%s", "infoScreen");
	blit_stringf(5, curline(), "%s", "Written by: Reza");
	blit_stringf(5, curline(), "%s", "credits to : OneRice07 , Rinnegatamante, Joshua");
	
}



