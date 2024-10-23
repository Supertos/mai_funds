// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos

// Exercise 5
// 13:22
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


size_t ckn( size_t k, size_t n ) {
	return fact_simple( n ) / fact_simple( n - k ) / fact_simple( k );
}

void polshift(double eps, double a, double** out, size_t n, ... ) {
	*out = calloc( n + 1, sizeof( double ) );
	if( !out ) return;
	
	va_list args;
	va_start( args, n );
	
	double* f = calloc( n + 1, sizeof( double ) );
	for( int i = 0; i < n + 1; i++ ) f[i] = va_arg( args, double );
	
	
	for( size_t i = 0; i < n + 1; i++ )
		for( size_t k = i; k < n + 1; k++ )
			(*out)[i] += f[k] * (double)ckn( i, k ) * pow( a, k - i );
	
	free(f);
	
	va_end( args );
}

void shitfunc( double* in, size_t n, double a, double x ) {
	long double out = 0;
	
	for( size_t z = 0; z <= n; z++ ) {
		out += in[z] * pow( x - a, z );
	}
	
	printf( "%Lf\n", out );
}

int main() {
	size_t n = 1;
	
	double* out;
	polshift( 0.0001, 5.0, &out, n, -25.0, 5.0 );
	for( size_t i = n + 1; i--; ) {
		printf( "%.0f ", (out)[i] );
	}
	printf( "\n" );
	
	shitfunc( out, 1, 5.0, 3.0 ); 
	
	free(out);
	
}