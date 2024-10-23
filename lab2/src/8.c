// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos

// Exercise 8
// 10:48
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


/* Returns min possible [char] notation */
int determineCharNotation( char c ) {
	if( c >= '0' && c <= '9' ) return c - '0' + 1;
	if( c >= 'A' && c <= 'Z' ) return c - 'A' + 11;
	if( c >= 'a' && c <= 'z' ) return c - 'a' + 11;
	if( c == '-' ) return 0;
	if( c == '\0' ) return 1;
	return -1;
}

char addToChar( char a, char b ) {
	int worth_a = determineCharNotation( a ) - 1;
	int worth_b = determineCharNotation( b ) - 1;
	if( worth_a == -2 || worth_b == -2 ) return 0;
	
	if( worth_a + worth_b > 9 ) return 'A' + worth_a + worth_b - 10;
	
	return '0' + worth_a + worth_b;
}
char addToCharWorth( char a, int worth_b ) {
	int worth_a = determineCharNotation( a ) - 1;
	if( worth_a == -2 || worth_b == -2 ) return 0;
	
	if( worth_a + worth_b > 9 ) return 'A' + worth_a + worth_b - 10;
	
	return '0' + worth_a + worth_b;
}
char setWorth( int worth ) {	
	if( worth > 9 ) return 'A' + worth - 10;
	
	return '0' + worth;
}


void sumdigit( int base, char* a, char* b ) {
	int worth_a = determineCharNotation( *a ) - 1;
	int worth_b = determineCharNotation( *b ) - 1;
	
	if( worth_a == -2 || worth_b == -2 || worth_a >= base || worth_b >= base ) return;
	if( worth_a + worth_b >= base ) {
		*a = setWorth( worth_a + worth_b - base );
		
		char temp = '1';
		
		sumdigit( base, (a - 1), &temp );
	}else{
		*a = addToChar( *a, *b );
	}
}

char* sumb( int base, string* a, char* b ) {
	size_t blen = Len(b);
	size_t alen = a->length;
	
	
	size_t newlen = alen > blen ? alen : blen;
	newlen += 2;
	
	if( !resizefrontstring( a, newlen ) ) return false;
	alen = a->length;
	
	size_t minlen = alen < blen ? alen : blen;
	for( size_t digit = 1; digit <= minlen; digit++ ) { // str[newlen] == '\0'
		sumdigit( base, &a->data[alen - digit], &b[ blen - digit ] );
	}
}

string* sum( int base, size_t numerochki, ... ) {
	if( base < 2 || base > 36 ) return NULL;
	
	va_list args;
	va_start( args, numerochki );
	
	string* out = newstring();
	
	for( ; numerochki--; ) {
		sumb( base, out, va_arg( args, char* ) );
	}
	
	while( out->data[0] == '0' || out->data[0] == '\0' ) shiftstring( out, 0 );
	
	va_end( args );
	return out;
}

int main() {
	string* out = sum(16, 4, "FF", "1", "FF", "000CC");
	
	
	
	for( size_t n = 0; n < out->length; n++ ) {
		printf( "%c", out->data[n] );
	}
	printf("\n");
	
	deletestring(out);
	
}