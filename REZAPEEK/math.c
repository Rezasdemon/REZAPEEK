#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "math.h"
 /* reverse:  reverse string s in place */
 void reverse(unsigned char s[],int length)
 {
     int i, j;
    unsigned char c;
 
     for (i = 0, j = length-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
 

int gettypesize(enum types t){
	switch(t)
	{
		case t_byte:
		return 1;
		break;
		case t_short:
		return 2;
		break;
		case t_int:
		return 4;
		break;
		case t_float:
		return 4;
		break;
		
	}
	return 0;
}

char* gettypename(enum types t)
{
	
	switch(t)
	{
		case t_byte:
		return "Byte";
		break;
		case t_short:
		return "Short";
		break;
		case t_int:
		return "Int";
		break;
		case t_float:
		return "Float";
		break;
		
	}
	return "";
	
}

//if no end is found it returns -1 check against it to avoid errors.
int getstrlength(char* str , int size){

int i;
for (i=0;i<size;i++)
{
	
	if ( str[i] == 0x00 ) {
		return i ;
	}
	
}

return -1;	

}

void inttostring(uint _Val, char* output )
    {  
		memset(output,0,10);
        snprintf(output,10, "%d", _Val);
		//itoa(val , buffer,10);
		
	}
	
void hexinttostring(uint i,int leading,char* output){
	switch(leading){
		case 1:
		snprintf(output, 11, "0x%01X",i);
		break;
		case 2:
		snprintf(output, 11, "0x%02X",i);
		break;
		case 4:
		snprintf(output, 11, "0x%04X",i);
		break;
		case 8:
		snprintf(output, 11, "0x%08X",i);
		break;
		//itoa(val , buffer,16);
		
	}
	
}
