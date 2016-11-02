#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

 /* reverse:  reverse string s in place */
 /*void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }
 
 // itoa:  convert n to characters in s 
 void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  // record sign 
         n = -n;          // make n positive 
     i = 0;
     do {       // generate digits in reverse order 
         s[i++] = n % 10 + '0';   // get next digit 
     } while ((n /= 10) > 0);     // delete it 
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }
*/

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
/*
int strleadzero(char* str ,size,int typesize)
{
	// str = 3 bytes allocated  size is 10  
	int len = getstrlength(str);
	int i;
	for (i=0;i<len ; i++)
	{
		
		
	}
	
	
}
*/
int inttostring(int _Val, char* output )
    {  
		memset(output,0,2*9);
        snprintf(output,2*9, "%d", _Val);
        return 0;
}
void hexinttostring(int i,char* output){
	snprintf(output, 11, "0x%08X",i);
}
