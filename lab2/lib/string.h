// -=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-
// Author: Supertos
// 
// String manipulation library for Generic OS
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#pragma once

#define EOF (-1)
#define _SYS_CDEFS_H 1
#define _STDIO_H 1

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#define IS_DIGIT(c) (c >= '0' && c <= '9')
#define IS_LETTER(c) (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')

/* Prints unsigned int [n] to [s] buffer in [base] notation */
void Uitoa( uint32_t n, char s[], uint8_t base );

/* Copies chunk of data to another location */
char* Copy( char* from, char* to, size_t n );

/* Reversed byte order in null-terminated string */
char* Reverse( char* s, size_t n );

/* Prints to [buf] variables with given [format] */
char* Format( char buf[], char* format, ... );

/* Returns length of null-terminated string */
size_t Len( const char* s );

/* Returns first encounter of substring [what] of size [n] in memory chunk [trg] of size [nn] */
char* Find( char* trg, char* what, size_t n, size_t nn ); // Returns pointer to the first encounter of what in trg.

/* Scans [buf] and fills given variables based on [format] */
void Scan( char* buf, char* restrict format, ... );

/* Prints signed int [n] to [s] buffer in [base] notation */
void Itoa(int n, char s[], uint8_t base);

/* Helper exponent functions (TODO: Remove from string.h) */
float EXP(float y);
float LOG(float y);
float POW(float b, float p);