// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos
//
// Exercise 4
// 18:58
// =-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>

#include "commandline.h"
#include "memman.h"
#include "string.h"
#include "vector.h"
#include "utils.h"
#include "number.h"

#define EPS 0.000001

typedef struct point point;
struct point {
	long double x;
	long double y;
};

DEFINE_VECTOR(point, vec)
DEFINE_VECTOR(char*, vec2)


point cgetvec( vec* obj, long int id ) {
	if( id < 0 )
		return cgetvec( obj, obj->length + id );
	
	if( id >= obj->length )
		return cgetvec( obj, id - obj->length );
	
	return obj->data[id];
}

bool isconvex( size_t i, ... ) {
	if( i == 0 ) return false;
	
	va_list args;
	va_start( args, i );
	
	vec* out = newvec();
	for( size_t n = i; n > 0; n-- ) 
		pushvec( out, va_arg( args, point ) );
	
	int sign = 0;
	for( size_t n = 0; n < i; n++ ) {
		point cur = cgetvec(out, n);
		point prev = cgetvec(out, n - 1);
		point next = cgetvec(out, n + 1);
		
		
		point ab = (point){ .x = cur.x - prev.x, .y = cur.y - prev.y };
		point bc = (point){ .x = next.x - cur.x, .y = next.y - cur.y };
		
		long int product = ab.x * bc.y - ab.y * bc.x;
		
		if( !sign ) sign = product >= EPS ? 1 : -1;
		
		if( (product >= EPS ? 1 : -1) != sign ) { deletevec(out); va_end( args ); return false; }
	}
	
	va_end( args );
	deletevec(out);
	return true;
}

long double compute( long double x, long int n, ... ) {
	if( n < 0 ) return NAN;
	
	va_list args;
	va_start( args, n );
	
	long double out = 0;
	
	for( ; n >= 0; n-- ) {
		long double val = va_arg( args, long double );
		out += powl( x, n ) * val;
	}
	
	va_end( args );
	return out;
}

/* Returns min possible [char] notation */
int determineCharNotation( char c ) {
	if( c >= '0' && c <= '9' ) return c - '0' + 1;
	if( c >= 'A' && c <= 'Z' ) return c - 'A' + 11;
	if( c >= 'a' && c <= 'z' ) return c - 'a' + 11;
	// if( c == '-' ) return 0;
	return -1;
}


size_t Atoi2( char* buf, size_t n, size_t base ) {
    size_t out = 0;
    for( size_t i = 0; i < n; i++ ) {
		if( determineCharNotation( buf[i] ) == -1 ) return 0;
        out += pow( base, n - i - 1 ) * (determineCharNotation( buf[i] ) - 1);
	}
	
    return out;
}

vec2* compute2( unsigned int base, size_t n, ... ) {
	if( n < 0 ) return NULL;
	if( base > 35 || base < 2 ) return NULL;
	
	va_list args;
	va_start( args, n );
	
	vec2* out = newvec2();
	
	for( ; n > 0; n-- ) {
		char* num = va_arg( args, char* );
		
		long int val = 0;
		
		for( size_t c = 0; c < Len(num); c++ ) {
			if( determineCharNotation( num[c] ) == -1 || determineCharNotation( num[c] ) > base ) { free(out); return NULL; }
			val += pow( base, Len(num) - c - 1 ) * ( determineCharNotation( num[c] ) - 1 );
		}
		
		if( powl( INT_MAX, 0.5L ) < val ) { free(out); return NULL; }
		long int sqr = pow( val, 2 );
		
		char buf[256];
		Itoa( sqr, buf, base );
		
		// Try all splits
		for( size_t p = 1; p <= Len(buf); p++ ) {
			unsigned long int a = Atoi2( buf, p, base ) + Atoi2( &buf[p], Len(buf) - p, base );
			
			if( a == val ) {
				pushvec2( out, num );
				break;
			}
		}
	}
	
	
	va_end( args );
	return out;
}

int main() {
	printf( "%d\n", isconvex( 4, (point){4.0, 0.0}, (point){4.0, 0.0}, (point){1.0, 2.0}, (point){0.0, 4.0} ) );
	printf( "%Lf\n", compute( 0, -3, 1.0L ) );
	
	vec2* out = compute2( 10, 5, "001", "9", "3", "45", "99" );
	if( !out ) return 0;
	
	for( size_t n = 0; n < out->length; n++ )
		printf("%s\n", out->data[n]);
	
	deletevec2( out );
	
	
	return 0;
}