// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos

// Exercise 5
// 13:11
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

#define EPS 0.000000001

DEFINE_VECTOR(unsigned long, uivector)

unsigned long gcd(unsigned long a, unsigned long b) {

    if(b == 0) {
            return a;
    }
    else {
        return gcd(b, a % b);
    }
}

uivector* factors( unsigned long num ) {
	uivector* vec = newuivector();
	
	for( unsigned long n = 2; n <= num; n++ ) {
		if( num % n ) continue;
		
		if( !pushuivector( vec, n ) ) return NULL;
		while( num / n ) num /= n;
	}		
	
	return vec;	
}

int finrepr( size_t radix, double n ) {
	unsigned long val;
	unsigned long denominator = 1;
	
	while( fabs(n - floor(n)) > EPS ) {
		if( denominator * 10 < denominator ) return -1;
		n *= 10;
		denominator *= 10;
	}
	
	
	val = (unsigned long)n;
	
	unsigned long gc;
	while( (gc=gcd( val, denominator )) != 1 ) {
		val /= gc;
		denominator /= gc;
	}
	
	denominator = round( (long double)denominator / (long double)val ); // Since we can't actually represent e.g. 1/3 in double, this will do	
	// printf( "Denominator: %zu\n", denominator );
	
	uivector* fct = factors( radix );
	
	for( size_t i = 0; i < fct->length; i++ ) {
		unsigned long factor = fct->data[i];
		
		while( !(denominator % factor) ) denominator /= factor;
	}
	deleteuivector( fct );
	
	return denominator == 1;
}

void finreprs( int* result, size_t radix, size_t n, ... ) {
	if( radix < 2 || radix > 36 || !n ) return;
	va_list args;
	va_start( args, n );
	
	for( size_t a = 0; a < n; a++ ) {
		double val = va_arg( args, double );
		result[a] = finrepr( radix, val );
	}
	
	va_end( args );
	return;	
}

int main() {
	int* out = calloc( 5, sizeof(int) );
	
	finreprs( out, 3, 1, 1.0 / 243.0);
	
	for( size_t a = 0; a < 1; a++ )
		printf( "%d\n", out[a] );
	printf( "\n" );
	
	
}

