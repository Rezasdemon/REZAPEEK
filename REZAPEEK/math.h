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
void reverse(unsigned char s[],int length);
int gettypesize(enum types t);
char* gettypename(enum types t);
int getstrlength(char* str , int size);
void inttostring(uint _Val, char* output);
void hexinttostring(uint i,int leading,char* output);

#endif
