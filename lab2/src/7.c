// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos

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

double dichotomy(double f(double), double a, double b, double eps) {
	double l = f(a);
	double r = f(b);
	
	double out;
	
	if( l * r > 0 ) return NAN; // Invalid func.

	while (fabs(a - b) > eps) {
		double mid = (a + b) / 2;
		
		if (fabs(f(mid) * l) < eps) { 
		if( isinf(f(mid)) || isnan(f(mid)) ) return NAN;
		return mid; }
		if (f(mid) * l > 0) {
			a = mid;
		} else {
			b = mid;
		}
	}

	// out = (a + b) / 2;
	return NAN;
}

double generic_func( double val ) {
	return pow(val-1, 2)/(val-1);
}

int main() {
	printf( "%f\n", dichotomy( generic_func, 0,2, 0.000000000001 ) );
}