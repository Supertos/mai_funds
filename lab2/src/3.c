// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos
//
// Exercise 3
// 13:46
// =-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "commandline.h"
#include "memman.h"
#include "string.h"
#include "vector.h"
#include "utils.h"
#include "number.h"

typedef struct found found;
struct found {
	ssize_t col;
	ssize_t row;
	
	char* filename;
};

#define INVALID_FOUND (found){.col = -1, .row = -1}

DEFINE_VECTOR(found, fvec)

found searchinfile( char* str, FILE* file, size_t colb, size_t rowb ) {
	size_t offset = 0;
	
	size_t col = colb;
	size_t row = rowb;
	
	size_t retcol = 0;
	size_t prevcol = 0;
	size_t retrow = 0;
	char c;
	while( offset < Len(str) && (c=fgetc(file)) != EOF ) {
		
		if( c == '\n' ) { row++; prevcol = col; col = 1; }else{ col++; }
		if( c == '\t' ) { col += 3; }
		if( str[offset] == c ) {
			if( !offset ) { retcol = col; retrow = row; }
			offset++;
		}else{
			fseek( file, -offset, SEEK_CUR );
			if( offset ) {
				col = retcol;
				row = retrow;
			}
			offset = 0;
		}
	}
	
	if( offset < Len(str) ) return INVALID_FOUND;
	fseek( file, -offset + 1, SEEK_CUR );
	retcol--;
	if( retcol == 0 ) {
		retrow--;
		retcol = prevcol;
	}
	return (found){.col = retcol, .row = retrow};
}

fvec* searchinfiles( char* str, size_t num, ... ) {
	if( num == 0 ) return NULL;
	va_list args;
	va_start( args, num );
	
	fvec* out = newfvec();
	for( ; num > 0; num-- ) {
		char* filename = va_arg(args, char*);
		FILE* file = fopen(filename, "r");
		if( !file ) { deletefvec(out); return NULL; }
		
		size_t colb = 1;
		size_t rowb = 1;
		found obj;
		do {
			obj = searchinfile( str, file, colb, rowb );
			obj.filename = filename;
			if( obj.col != -1 ) pushfvec( out, obj );
			colb = obj.col + 1;
			rowb = obj.row;
		} while( obj.col != -1 );
		
		fclose( file );
	}
	
	va_end( args );
	return out;
}


int main( int argc, char** argv ) {
	
	char* str = " \r\n \r\n";
	
	fvec* list = searchinfiles( str, 2, "ai_templates.lua", "123.txt" );
	if( !list ) return 0;
	for( size_t n = 0; n < list->length; n++ ) {
		printf( "\"%s\" at %zu:%zu in %s\n", str, list->data[n].row, list->data[n].col, list->data[n].filename );
	}
	
	deletefvec( list );
}

