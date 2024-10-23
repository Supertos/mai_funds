// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos

// Exercise 5
// 11:33
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
		int* temp = va_arg( args, int* );
		*temp = offset;
		return 0;
	}
	
	ssize_t printed = 0;
	
	if( target.type == TargetFile ) {
		printed = vfscanf( target.target.file, format2, args );
	}else{
		printed = vsscanf( &target.target.str[offset], format2, args );
	}		
	
	free( format2 );
	return printed;
}

ssize_t scanchar( struct OutputTarget target, char* c, size_t offset ) {
	if( target.type == TargetFile ) {
		fscanf( target.target.file, "%c", c );
	}else{
		ssize_t res = sscanf( &(target.target.str[offset]), "%c", c  );
		if( res == -1 ) return -1;
	}
	return 1;
}
ssize_t scanstr( struct OutputTarget target, char* c, size_t offset ) {
	if( target.type == TargetFile ) {
		return fscanf( target.target.file, "%s", c );
	}else{
		return sscanf( &target.target.str[offset], "%s", c );
	}
}
ssize_t scanint( struct OutputTarget target, int* c, size_t offset ) {
	if( target.type == TargetFile ) {
		return fscanf( target.target.file, "%d", c );
	}else{
		return sscanf( &target.target.str[offset], "%d", c );
	}
}
ssize_t moveback( struct OutputTarget target ) {
	if( target.type == TargetFile )
		fseek( target.target.file, -1, SEEK_CUR );
	return -1;
}

ssize_t scanrom( struct OutputTarget target, size_t offset, int* val ) {
	if( !val ) return -1;
	int i = 0;
	int result = 0;
	int mul = 1;
	
	size_t printed = 0;
	
	char roman[2];
	roman[0] = '\0';
	*val = 0;
	
	ssize_t read;
	
	do {
		char temp;
		read = scanchar( target, &temp, offset + printed++ );
		roman[1] = temp;
		switch( roman[i] ) {
			case '-': mul = -1;
			case 'I': result += (roman[i + 1] == 'V' || roman[i + 1] == 'X' ? -1 : 1); break;
			case 'V': result += 5; break;
			case 'X': result += (roman[i + 1] == 'L' || roman[i + 1] == 'C' ? -10 : 10); break;
			case 'L': result += 50; break;
			case 'C': result += (roman[i + 1] == 'D' || roman[i + 1] == 'M' ? -100 : 100); break;
			case 'D': result += 500; break;
			case 'M': result += 1000; break;
			default: break;
		}
		roman[0] = roman[1];
		if( read == -1 ) break;
	} while(  roman[i] == 'I' || roman[i] == 'V' || roman[i] == 'X' || roman[i] == 'L' || roman[i] == 'C' || roman[i] == 'D' || roman[i] == 'M' );
	
	*val = result * mul;
	
	if( read != -1 ) printed += moveback( target );
	
	return printed;
}

const unsigned int FIB[] = {1,         2,         3,         5,          8,          13,        21,        34,
                            55,        89,        144,       233,        377,        610,       987,       1597,
                            2584,      4181,      6765,      10946,      17711,      28657,     46368,     75025,
                            121393,    196418,    317811,    514229,     832040,     1346269,   2178309,   3524578,
                            5702887,   9227465,   14930352,  24157817,   39088169,   63245986,  102334155, 165580141,
                            267914296, 433494437, 701408733, 1134903170, 1836311903, 2971215073};

ssize_t scanzek( struct OutputTarget target, size_t offset, unsigned int* val ) {
	if( !val ) return -1;
	const int nFibs = sizeof(FIB) / sizeof(FIB[0]);
	size_t printed = 0;
	
	
	ssize_t totlen = 0;
	ssize_t read = 0;
	*val = 0;
	char c;
	for( size_t n = 0; n < nFibs + 1; n++ ) {
		read = scanchar( target, &c, offset + printed++ );
		if( c != '0' && c != '1' || read == -1 ) break;
		totlen++;
	}
	
	if( totlen > nFibs ) { if( read != -1 ){ moveback( target ); } return printed - 1; }
	while( printed ) printed += moveback( target );
	
	
	for( size_t n = 0; n < nFibs + 1; n++ ) {
		if( n >= nFibs ) { *val = 0; return -1; }
		read = scanchar( target, &c, offset + printed++ );
		if( c != '0' && c != '1' || read == -1 ) break;
		*val += FIB[n] * ( c == '1' ? 1 : 0 );
	}
	
	if( read != -1 ) printed += moveback( target );
	
	return printed;
	
}

const char* ITOA_ALPHABET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* Returns min possible [char] notation */
int determineCharNotation( char c ) {
	if( c >= '0' && c <= '9' ) return c - '0' + 1;
	if( c >= 'A' && c <= 'Z' ) return c - 'A' + 11;
	if( c >= 'a' && c <= 'z' ) return c - 'a' + 11;
	if( c == '-' ) return 0;
	return -1;
}

ssize_t printbased( struct OutputTarget target, size_t offset, bool upper, va_list args ) {
	size_t printed = 0;
	
	int* val = va_arg( args, int* );
	int base = va_arg( args, int );
	if( !val ) return -1;
	
	if( base > 36 || base < 2 ) base = 10;
	
	ssize_t totlen = 0;
	ssize_t read = 0;
	ssize_t mul = 1;
	*val = 0;
	char c;
	for( size_t n = 0; true; n++ ) {
		read = scanchar( target, &c, offset + printed++ );
		int worth = determineCharNotation( c ) - 1;
		
		if( worth == -2 ){ break; }
		if( worth >= base ){ break; }
		
		totlen++;
	}
	
	while( printed ) printed += moveback( target );
	
	for( size_t n = 0; n < totlen; n++ ) {
		read = scanchar( target, &c, offset + printed++ );
		
		int worth = determineCharNotation( c ) - 1;
		
		if( worth == -2 ){ break; }
		if( worth >= base ){ break; }
		
		if( c == '-' ) {
			if( mul == -1 ) { *val = 0; return -1; }
			mul = -1;
		}
			
		
		*val += pow( base, totlen - n ) * worth;
	}
	
	return printed;
}

ssize_t executenonstdspec( struct OutputTarget target, char* format, size_t begin, size_t end, size_t offset, va_list args ) {
	char* format2 = substring( format, begin, end );
	
	if( !strcmp(format2, "%Ro") ) { free( format2 );return scanrom( target, offset, va_arg( args, int* ) );}
	if( !strcmp(format2, "%Zr") ) { free( format2 );return scanzek( target, offset, va_arg( args, unsigned int* ) );}
	if( !strcmp(format2, "%Cv") ) { free( format2 );return printbased( target, offset, false, args );}
	if( !strcmp(format2, "%CV") ) { free( format2 );return printbased( target, offset, true, args );}
	
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
			char temp;
			scanchar( target, &temp, printed++ );
			if( format[cursor++] != temp ) return 0;
		}
	}
	return printed;
}

size_t oversscanf( char* buf, char* format, ... ) {
	va_list args;
	va_start( args, format );
	struct OutputTarget trg;
	trg.target.str = buf;
	trg.type = TargetString;
	
	size_t ret = parseformat( trg, format, args );
	va_end( args );
	return ret;
}

size_t overfscanf( FILE* buf, char* format, ... ) {
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
	char test[51200] = "cake satan 001001-FFXIV nya!cake!anime!";
	char test5[51200] = "";
	
	int test2 = 0;
	int test3 = 0;
	int test4 = 0;
	
	oversscanf(test, "cake satan %Zr%Cv%Ro %4s", &test2, &test4, 16, NULL, &test5);
	
	printf( "%s\n", test );
	printf( "%d\n", test2 );
	printf( "%d\n", test4 );
	printf( "%d\n", test3 );
	printf( "%s\n", test5 );
	
}