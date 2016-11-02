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
void hexinttostringnoleading(uint i,int leading,char* output);


//! Byte swap unsigned short
uint16_t swap_uint16( uint16_t val );

//! Byte swap short
int16_t swap_int16( int16_t val ) ;

//! Byte swap unsigned int
uint32_t swap_uint32( uint32_t val );

//! Byte swap int
int32_t swap_int32( int32_t val );

//! Byte swap long
int64_t swap_int64( int64_t val );

//! Byte swap unsigned long 
uint64_t swap_uint64( uint64_t val );



#endif

