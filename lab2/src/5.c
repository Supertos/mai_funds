// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos
//
// Exercise 5
// 21:50
// =-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include "commandline.h"
#include "memman.h"
#include "string.h"
#include "vector.h"
#include "utils.h"
#include "number.h"

DEFINE_VECTOR(char, string);

enum OutputTargetType {
	TargetFile = 1,
	TargetString = 2
};

struct OutputTarget {
	enum OutputTargetType type;
	
	union {
		FILE* file;
		char* str;
	} target;
};

ssize_t expectflag( char* format, size_t begin ) {
	switch( format[begin] ) {
		case '#':
		case '0':
		case '-':
		case ' ':
		case '+':
			return begin + 1;
		default:
			return -1;
	}
}


ssize_t expectfieldwidth( char* format, size_t begin ) {
	if( format[begin] == '0' ) return -1; // POSIX disallows.
	
	if( format[begin] == '*' ) {
		if( !IS_DIGIT( format[begin + 1] ) ) return begin + 1;
		begin++;
		while( IS_DIGIT( format[begin] ) ) begin++;
		
		if( format[begin] != '$' ) return -1;
		return begin + 1;
	}
	
	if( !(IS_DIGIT(format[begin])) ) return -1; // POSIX disallows.
	
	begin++;
	while( IS_DIGIT( format[begin] ) ) begin++;
	
	return begin;
}

ssize_t expectprecision( char* format, size_t begin ) {
	if( format[begin] != '.' ) return -1;
	
	if( format[begin] == '*' ) {
		if( !IS_DIGIT( format[begin + 1] ) ) return begin + 1;
		begin++;
		while( IS_DIGIT( format[begin] ) ) begin++;
		
		if( format[begin] != '$' ) return -1;
		return begin + 1;
	}
	
	begin++;
	while( IS_DIGIT( format[begin] ) ) begin++;
	
	return begin;
}
ssize_t expectlenmodifier( char* format, size_t begin ) {
	switch( format[begin] ) {
		case 'L':
		case 'j':
		case 'z':
			return begin + 1;
		case 'l':
		case 'h':
			if( format[begin+1] == format[begin] ) return begin + 2;
			return begin + 1;
	}
	return -1;
}
ssize_t expectconvspec( char* format, size_t begin ) {
	switch( format[begin] ){
		case 'd':
		case 'i':
		case 'o':
		case 'u':
		case 'x':
		case 'X':
		case 'e':
		case 'E':
		case 'f':
		case 'F':
		case 'g':
		case 'G':
		case 'a':
		case 'A':
		case 'c':
		case 's':
		case 'p':
		case 'n':
		case '%':
			return begin + 1;
		default:
			return -1;
	}
}

ssize_t expectconvspec2( char* format, size_t begin ) {
	if( format[begin] == 'R' && format[begin + 1] == 'o' ) return begin + 2;
	if( format[begin] == 'Z' && format[begin + 1] == 'r' ) return begin + 2;
	if( format[begin] == 'C' && format[begin + 1] == 'v' ) return begin + 2;
	if( format[begin] == 'C' && format[begin + 1] == 'V' ) return begin + 2;
	if( format[begin] == 't' && format[begin + 1] == 'o' ) return begin + 2;
	if( format[begin] == 'T' && format[begin + 1] == 'O' ) return begin + 2;
	if( format[begin] == 'm' && format[begin + 1] == 'i' ) return begin + 2;
	if( format[begin] == 'm' && format[begin + 1] == 'u' ) return begin + 2;
	if( format[begin] == 'm' && format[begin + 1] == 'd' ) return begin + 2;
	if( format[begin] == 'm' && format[begin + 1] == 'f' ) return begin + 2;
	
	return -1;
}

ssize_t expectspecifier( char* format, size_t begin ) {
	ssize_t out = begin;
	out = expectflag( format, out ) != -1 ? expectflag( format, out ) : out;
	out = expectfieldwidth( format, out ) != -1 ? expectfieldwidth( format, out ) : out;
	out = expectprecision( format, out ) != -1 ? expectprecision( format, out ) : out;
	out = expectlenmodifier( format, out ) != -1 ? expectlenmodifier( format, out ) : out;
	
	
	out = expectconvspec( format, out ) != -1 ? expectconvspec( format, out ) : out;
	if( out == begin ) return -1; // Fatal error!!!!!! Ouch!!!!
	return out;
}

ssize_t expectnonstdspecifier( char* format, size_t begin ) {
	return expectconvspec2( format, begin );
}

char* substring( char* format, size_t begin, size_t end ) {	
	char* format2 = malloc( end - begin + 1 );
	strncpy( format2, &format[begin], end - begin );
	format2[end-begin] = '\0';
	
	return format2;
}

ssize_t executestdspec( struct OutputTarget target, char* format, size_t begin, size_t end, size_t offset, va_list args ) {
	char* format2 = substring( format, begin, end );
	
	if( !strcmp( format2, "%n" ) ) {
		free( format2 );
		int* temp = va_arg( args, int* );
		*temp = offset;
		return 0;
	}
	
	ssize_t printed = 0;
	
	if( target.type == TargetFile ) {
		printed = vfprintf( target.target.file, format2, args );
	}else{
		printed = vsprintf( &target.target.str[offset], format2, args );
	}		
	
	free( format2 );
	return printed;
}

ssize_t printchar( struct OutputTarget target, char c, size_t offset ) {
	if( target.type == TargetFile ) {
		fprintf( target.target.file, "%c", c );
	}else{
		sprintf( &target.target.str[offset], "%c", c );
	}
	return 1;
}
ssize_t printstr( struct OutputTarget target, char* c, size_t offset ) {
	if( target.type == TargetFile ) {
		return fprintf( target.target.file, "%s", c );
	}else{
		return sprintf( &target.target.str[offset], "%s", c );
	}
}
ssize_t printint( struct OutputTarget target, int c, size_t offset ) {
	if( target.type == TargetFile ) {
		return fprintf( target.target.file, "%d", c );
	}else{
		return sprintf( &target.target.str[offset], "%d", c );
	}
}

ssize_t printrom( struct OutputTarget target, size_t offset, int val ) {
	if( val > 10000 ) return -1;
	size_t printed = 0;
	
	if( val == 0 ) { printchar( target, '0', offset + printed++ ); }
	
	if( val < 0 ) {
		printchar( target, '-', offset + printed++ );
		val *= -1;
	}
	
	while( val >= 1000 ) {
		printchar( target, 'M', offset + printed++ ); 
		val -= 1000;
	}
	while( val >= 500 ) {
		printchar( target, 'D', offset + printed++ ); 
		val -= 500;
	}
	while( val >= 100 ) {
		printchar( target, 'C', offset + printed++ ); 
		val -= 100;
	}
	while( val >= 50 ) {
		printchar( target, 'L', offset + printed++ ); 
		val -= 50;
	}
	while( val >= 10 ) {
		printchar( target, 'X', offset + printed++ ); 
		val -= 10;
	}
	while( val >= 5 ) {
		printchar( target, 'V', offset + printed++ ); 
		val -= 5;
	}
	while( val >= 1 ) {
		printchar( target, 'I', offset + printed++ ); 
		val -= 1;
	}
	
	return printed;
}

const unsigned int FIB[] = {1,         2,         3,         5,          8,          13,        21,        34,
                            55,        89,        144,       233,        377,        610,       987,       1597,
                            2584,      4181,      6765,      10946,      17711,      28657,     46368,     75025,
                            121393,    196418,    317811,    514229,     832040,     1346269,   2178309,   3524578,
                            5702887,   9227465,   14930352,  24157817,   39088169,   63245986,  102334155, 165580141,
                            267914296, 433494437, 701408733, 1134903170, 1836311903, 2971215073};

ssize_t printzek( struct OutputTarget target, size_t offset, int val ) {
	uint64_t zeckendorf = 0;
	const int nFibs = sizeof(FIB) / sizeof(FIB[0]);

	for (size_t i = nFibs - 1; i--;) {
		if (FIB[i] <= val) {
			zeckendorf |= (1ULL << i);
			val -= FIB[i];
		}
	}

	size_t printed = 0;
	while (zeckendorf) {
		printchar(target, zeckendorf & 1 ? '1' : '0', offset + printed++);

		zeckendorf >>= 1;
	}

	printchar(target, '1', offset  + printed++);
	return printed;
	
}
ssize_t moveback( struct OutputTarget target ) {
	if( target.type == TargetFile )
		fseek( target.target.file, -1, SEEK_CUR );
	return -1;
}


const char* ITOA_ALPHABET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

ssize_t printbased( struct OutputTarget target, size_t offset, bool upper, va_list args ) {
	
	char s[32];
	unsigned int base = va_arg( args, unsigned int );
	int n = va_arg( args, int );
	
	uint8_t i = 0;
	int sign = n < 0 ? -1 : 1;
 
    if(sign < 0) n = -n;
	
    do {      
        s[i++] = upper || n % base < 10 ? ITOA_ALPHABET[ n % base ] : ITOA_ALPHABET[ n % base ] + 32; 
    }while( (n /= base) > 0 );
    
    if( sign < 0 ) s[i++] = '-';
	
    s[i] = '\0';
    Reverse(s, i);
	
	return printstr( target, s, offset );
}

/* Returns min possible [char] notation */
int determineCharNotation( char c ) {
	if( c >= '0' && c <= '9' ) return c - '0' + 1;
	if( c >= 'A' && c <= 'Z' ) return c - 'A' + 11;
	if( c >= 'a' && c <= 'z' ) return c - 'a' + 11;
	if( c == '-' ) return 0;
	return -1;
}

ssize_t printrebased( struct OutputTarget target, size_t offset, bool upper, va_list args ) {
	char* buf = va_arg( args, char* );
	unsigned int base = va_arg( args, unsigned int );
	
	size_t n = Len(buf);
	
    int out = 0;
	int sign = 1;
    for( size_t i = 0; i < n; i++ ) {
		if( determineCharNotation( buf[i] ) == -1 ) return -1;
		if( buf[i] == '-' ) {
			if( sign == -1 ) return -1;
			sign = -1;
		}
		if( upper && buf[i] > 'Z' ) return -1;
		if( !upper && buf[i] < 'a' ) return -1;
		if( pow( base, n - i - 1 ) * (determineCharNotation( buf[i] ) - 1) + out < out ) return -1;
        out += pow( base, n - i - 1 ) * (determineCharNotation( buf[i] ) - 1);
	}
	
	return printint( target, out * sign, offset );
}

ssize_t dumpregion( struct OutputTarget target, size_t offset, void* trg, size_t size ) {
	
	size_t printed = 0;
	
	for( size_t n = 0; n < size; n++ ) {
		for( size_t i = 0; i < 8; i++ ) {
			printchar(target, (255 & ((uint8_t*)trg)[n]) ? '1' : '0', offset  + printed++);
			((uint8_t*)trg)[n] <<= 1;
		}
		printchar(target, ' ', offset  + printed++);
	}
	
	return printed;
	
}

ssize_t executenonstdspec( struct OutputTarget target, char* format, size_t begin, size_t end, size_t offset, va_list args ) {
	char* format2 = substring( format, begin, end );
	
	if( !strcmp(format2, "%Ro") ) { free( format2 );return printrom( target, offset, va_arg( args, int ) );}
	if( !strcmp(format2, "%Zr") ) { free( format2 );return printzek( target, offset, va_arg( args, int ) );}
	if( !strcmp(format2, "%Cv") ) { free( format2 );return printbased( target, offset, false, args );}
	if( !strcmp(format2, "%CV") ) { free( format2 );return printbased( target, offset, true, args );}
	if( !strcmp(format2, "%to") ) { free( format2 );return printrebased( target, offset, false, args );}
	if( !strcmp(format2, "%TO") ) { free( format2 );return printrebased( target, offset, true, args );}
	if( !strcmp(format2, "%mi") ) { free( format2 );int temp = va_arg(args, int); return dumpregion( target, offset, &temp, sizeof(int) );}
	if( !strcmp(format2, "%mu") ) { free( format2 );int temp = va_arg(args, unsigned int); return dumpregion( target, offset, &temp, sizeof(unsigned int) );}
	if( !strcmp(format2, "%md") ) { free( format2 );int temp = va_arg(args, double); return dumpregion( target, offset, &temp, sizeof(double) );}
	if( !strcmp(format2, "%mf") ) { free( format2 );int temp = va_arg(args, double); return dumpregion( target, offset, &temp, sizeof(float) );}
	
	
	free( format2 );
	return -1;
}

size_t parseformat( struct OutputTarget target, char* format, va_list args ) {
	
	size_t printed = 0;
	
	for( size_t cursor = 0; cursor < Len(format); ) {
		if( format[cursor] == '%' ) {
			ssize_t temp = expectspecifier( format, cursor + 1 );
			if( temp == -1 ) {
				temp = expectnonstdspecifier( format, cursor + 1 );
				if( temp == -1 ) { return 0; }
				
				printed += executenonstdspec( target, format, cursor, temp, printed, args );
			}else{
				ssize_t result = executestdspec( target, format, cursor, temp, printed, args );
				if( result == -1 ) { return 0; }
				printed += result;
			}
			
			cursor = temp;
		}else{
			printchar( target, format[cursor++], printed++ );
		}
	}
	return printed;
}

size_t oversprintf( char* buf, char* format, ... ) {
	va_list args;
	va_start( args, format );
	struct OutputTarget trg;
	trg.target.str = buf;
	trg.type = TargetString;
	
	size_t ret = parseformat( trg, format, args );
	va_end( args );
	return ret;
}

size_t overfprintf( FILE* buf, char* format, ... ) {
	va_list args;
	va_start( args, format );
	struct OutputTarget trg;
	trg.target.file = buf;
	trg.type = TargetFile;
	
	size_t ret = parseformat( trg, format, args );
	va_end( args );
	return ret;
}

int main() {
	char test[51200];
	
	int test2;
	
	// oversprintf(test, "this is a test %Zr %n", 16, &test2);
	overfprintf(stdout, "this is a test %s", "Nya!\n");
	overfprintf(stdout, "this is a test %Zr %n %Ro %Ro %Ro %Ro", 16, &test2, 10, 4321, 0, -15);
	// printf( "%s\n", test );
	// printf( "%d\n", test2 );
	
}