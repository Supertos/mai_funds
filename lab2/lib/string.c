// -=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-
// Author: Supertos
// 
// String manipulation library for Generic OS
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include "string.h"
#include <stdio.h>
#include <stdio.h>

#define EXP_A 184 
#define EXP_C 16249 

float EXP(float y) {
    union {
        float d;
        struct {
			short j, i;
		} n;
    } eco;
    eco.n.i = EXP_A*(y) + (EXP_C);
    eco.n.j = 0;
    return eco.d;
}

float LOG(float y) {
    int* nTemp = (int*)&y;
    y = (*nTemp) >> 16;
    return (y - EXP_C) / EXP_A;
}

float POW(float b, float p) {
	if( p < 1 ) return 1;
    return EXP(LOG(b) * p);
}

static char* ITOA_ALPHABET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char* Reverse( char* s, size_t n ) {
	char temp;
	for( size_t i = 0; i < n / 2; i++ ) {
		temp = s[i];
		s[i] = s[n-i-1];
		s[n-i-1] = temp;
	}
	return s;
}

char* Copy( char* from, char* to, size_t n ) {
	for( size_t i = 0; i < n; i++ )
	    *(to++) = *(from++);
	return to;
}

size_t Len( const char* s ) {
	size_t out = 0;
	while( *(s++) != '\0' ) out++;
	return out;
}

void Itoa(int n, char s[], uint8_t base) {
	uint8_t i = 0;
	int sign = n < 0 ? -1 : 1;
 
    if(sign < 0) n = -n;
	
    do {      
        s[i++] = ITOA_ALPHABET[n % base]; 
    }while( (n /= base) > 0 );
    
    if( sign < 0 ) s[i++] = '-';
	
    s[i] = '\0';
    Reverse(s, i);
}

void Uitoa(uint32_t n, char s[], uint8_t base) {
	uint8_t i = 0;
    do {      
        s[i++] = ITOA_ALPHABET[n % base]; 
    }while( (n /= base) > 0 );
    
	
    s[i] = '\0';
    Reverse(s, i);
}

char* Format( char* buf, char* restrict format, ... ) {
	va_list args;
	va_start(args, format);
	char* out = buf;
	
	char itoabuf[32];
	
	while( *(format) != '\0' ) {
		if( *(format) == '%' ) {
			switch( format[1] ) {
				
				case 'd':
					Itoa( va_arg( args, int ), itoabuf, 10 );
					buf = Copy( itoabuf, buf, Len(itoabuf) );
				break;
				case 'u':
					Uitoa( va_arg( args, size_t ), itoabuf, 10 );
					buf = Copy( itoabuf, buf, Len(itoabuf) );
				break;
				case 'c':
					*(buf++) = (char)va_arg( args, int );
				break;
				
				case 'p':
					Itoa( va_arg( args, size_t ), itoabuf, 16 );
					buf = Copy( itoabuf, buf, Len(itoabuf) );
				break;
				case 's':;
					char* str = va_arg( args, char* );
					buf = Copy( str, buf, Len(str) );
				break;
				
				default: // Invalid
				break;
			}
			format++;
		}else{
			*(buf++) = *(format);
		}
		format++;
	}
	va_end( args );
	return out;
}

char* Find( char* trg, char* what, size_t n, size_t nn ) {
	size_t offset = 0;
	size_t testpos = 0;
	
	while( offset < n ) {
		if( trg[offset] == what[testpos] ) {
			testpos++;
			offset++;
			if( offset > nn ){ return &trg[offset - testpos]; }
		}else{
			offset -= testpos;
			offset++;
		}
	}
	return 0;
}

static inline size_t Round( float x ) {
	return (size_t)( x + 0.5 );
}

size_t Atoi( char* buf, size_t n ) {
    size_t out = 0;
    for( size_t i = 0; i < n; i++ )
        out += Round(POW( 10, n - i - 1 )) * (buf[i] - '0');
	
    return out;
}

void Scan( char* buf, char* restrict format, ... ) {
	va_list args;
	va_start(args, format);
	char* out = buf;
	
	while( *(format) != '\0' ) {
		if( *(format) == '%' ) {
			switch( format[1] ) {
				case 'd': {
					int* target = va_arg( args, int* );
					    
					char atoibuf[128];
					size_t offset = 0;
					
					char c;
					do {
						c = *(buf++);
					    atoibuf[offset++] = c;
					} while( format[2] != c && c != '\0' && offset < 128 );
					
					*(target) = Atoi(atoibuf, offset - 1);
					format += 3;
					}
				break;
				case 'c':
					*(va_arg( args, char* )) = *(buf++);
					format += 3;
				break;
				case 's':{
					    char* target = va_arg( args, char* );
					    
					    char c = *(buf++);
					    size_t offset = 0;
					    while( format[3] != c ) {
					        target[offset++] = c;
						    c = *(buf++);
					    }
					    target[offset] = '\0';
					
					    format += 3;
					}
				break;
				default: // Invalid
				break;
			}
		}else if( *(format) != *(buf++) ){ break; } // Template and input differs!
		format++;
	}
}

