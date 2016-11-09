#ifndef UI 
#define UI 
enum CMenu {
    MAIN = 0,
	SEARCH,
	VIEWMEM,
	DATABASE,
	INFO
};
struct menuoptions {
	char* left ;
	char* right;
	int useline;
	int x;
	int y;
	
}; // sent to blit so left is formated string with right being the replace of %s in left


int imenu;
int imenu_opt;
void updatemenusbuf(int id);

char titleid[16], title[256];
char* getMenuName(int menuid);
char* menusbuf;
void displayMenu(int ram_mode,SceCtrlData pad, SceCtrlData oldpad);
#endif