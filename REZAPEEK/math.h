#ifndef MATH
#define MATH

//int soutputlen;
//char* soutput;
//void itoa(int n, char s[]);
enum types {
	t_byte,
	t_short,
	t_int,
	t_float
};

int gettypesize(enum types t);
char* gettypename(enum types t);
int getstrlength(char* str , int size);
void inttostring(int _Val, char* output);
void hexinttostring(int i,int leading,char* output);
#endif
