// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos
//
// Exercise 2
// 13:05
// =-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-

#include <stdio.h>

#include "commandline.h"
#include "memman.h"
#include "string.h"
#include "vector.h"
#include "utils.h"
#include "number.h"

#include <math.h>

double median( size_t amount, ... ) { // ... can be omitted, see C99 spec.
	if( amount == 0 ) return NAN;
	va_list args;
	va_start( args, amount );
	
	double med = 1;
	double div = amount;
	for( ;amount--; ) {
		double arg = va_arg( args, double );
		if( arg < 0 ) {
			va_end( args );
			return NAN;
		}
		med *= arg;
	}
	
	double result = pow( med, 1.0 / div );
	
	va_end( args );
	
	return result;
}

long double rpow( long double what, int exp ) {
	if( exp == -1 ) return 1 / what;
	if( exp == 1 ) return what;
	if( exp % 2 == 0 ) {
		return rpow( what, exp / 2) * rpow( what, exp / 2);
	} else {
		return rpow( what, exp / 2) * rpow( what, exp / 2) * (exp < 0 ? 1 / what : what);
	}
}

int main( int argc, char** argv ) {
	printf( "%f\n", median( 3, -1.0, 10.0, 15.0 ) );
	printf( "%Lf\n", rpow( 2, -2 ) );
	
	return 0;
} // return 0 implicit, see C99 spec.

/* SISP */