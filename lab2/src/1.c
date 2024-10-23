// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos
//
// Exercise 1
// 11:28
// =-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-

#include <stdio.h>

#include "commandline.h"
#include "memman.h"
#include "string.h"
#include "vector.h"
#include "utils.h"
#include "number.h"

DEFINE_VECTOR(char, string)
DEFINE_VECTOR(size_t, stvector)

void upper( char* c ) {
	if( *c >= 'a' && *c <= 'z' )
		*c -= 32;
}
int handleL( CMD* command, ProgramState* state ) {
	char* token = expectCMD( command, /* CMDType */ String );
	if( !token ) return INVALID_ARGUMENT_ORDER_ERR;
	
	printf( "%zu\n", Len(token) );
	
	return 0;
}

int handleR( CMD* command, ProgramState* state ) {
	char* token = expectCMD( command, /* CMDType */ String );
	if( !token ) return INVALID_ARGUMENT_ORDER_ERR;
	
	printf( "%s\n", Reverse(token, Len(token)) );
	return 0;
}

int handleU( CMD* command, ProgramState* state ) {
	char* token = expectCMD( command, /* CMDType */ String );
	if( !token ) return INVALID_ARGUMENT_ORDER_ERR;
	
	for( size_t n = 0; n < strlen(token); n++ )
		if( n % 2 == 0 ) upper( &token[n] ); // token[0] is the first symbol => upper even.

	printf( "%s\n", token );
	return 0;
}

int handleN( CMD* command, ProgramState* state ) {
	char* token = expectCMD( command, /* CMDType */ String );
	if( !token ) return INVALID_ARGUMENT_ORDER_ERR;
	
	string* digits = newstring();
	if( !digits ) return serror( OUT_OF_MEM_ERR, NULL );
	
	string* letters = newstring();
	if( !letters ) {
		deletestring( digits );
		return serror( OUT_OF_MEM_ERR, NULL );
	}
	
	string* others = newstring();
	if( !others ) {
		deletestring( digits );
		deletestring( letters );
		
		return serror( OUT_OF_MEM_ERR, NULL );
	}
	
	while( *token != '\0' ) {
		if( IS_DIGIT(*token) && !pushstring(digits, *token) ) { deletestring( digits ); deletestring( letters ); deletestring( others ); return serror( OUT_OF_MEM_ERR, NULL ); }
		if( IS_LETTER(*token) && !pushstring(letters, *token) ) { deletestring( digits ); deletestring( letters ); deletestring( others ); return serror( OUT_OF_MEM_ERR, NULL ); }
		if( !IS_DIGIT(*token) && !IS_LETTER(*token) && !pushstring( others, *token )) { deletestring( digits ); deletestring( letters ); deletestring( others ); return serror( OUT_OF_MEM_ERR, NULL ); }
		token++;
	} 
	if( !concatstring( digits, letters ) ) { deletestring( digits ); deletestring( letters ); deletestring( others ); return serror( OUT_OF_MEM_ERR, NULL ); }
	if( !concatstring( digits, others ) ) { deletestring( digits ); deletestring( letters ); deletestring( others ); return serror( OUT_OF_MEM_ERR, NULL ); }
	pushstring( digits, '\0' );
	
		deletestring( digits );
		deletestring( letters );
		deletestring( others );
	printf( "%s\n", digits->data );
	
	return 0;
}

int populateRandoms( stvector* obj, size_t a, size_t b ) {
	for( size_t n = a; n < b; n++ )
		if( !pushstvector( obj, n ) ) return 1;
	return 0;
}

int handleC( CMD* command, ProgramState* state ) {
	char* token = expectCMD( command, /* CMDType */ Int );
	if( !token ) return INVALID_ARGUMENT_ORDER_ERR;
	
	srand( (unsigned int)cstrtod( token ) );
	
	size_t argc = command->argc;
	size_t begin = 3; // filename, flag, seed
	size_t stringnum = argc - begin;
	
	string* out = newstring();
	if( !out ) return serror( OUT_OF_MEM_ERR, NULL );
	
	stvector* randoms = newstvector();
	if( !randoms || !resizestvector( randoms, stringnum ) ) {
		deletestring( out );
		return serror( OUT_OF_MEM_ERR, NULL );
	}
	
	populateRandoms( randoms, begin, argc );
	
	for( size_t n = command->argc - 3; n > 0; n-- ) {
		size_t id = (size_t)RANDOM(0, randoms->length - 1);
		char* str = command->argv[ shiftstvector( randoms, id ) ];
		if( !pusharrstring( out, str, strlen(str) ) ) {
			deletestring( out );
			deletestvector( randoms );
			return serror( OUT_OF_MEM_ERR, NULL );
		}
	}
	
	pushstring( out, '\0' );
	
	printf( "%s\n", out->data );
	
	deletestring( out );
	deletestvector( randoms );
	// Flush CMD
	while( expectCMD( command, /* CMDType */ String ) );
	
	return 0;
}

int main( int argc, char** argv ) {
	/* Handle array. Mind to update handlec */
	size_t handlec = 5;
	FlagHandle handles[handlec];
	handles[0] = (FlagHandle){ .handle = handleL, .flag = "-L" };
	handles[1] = (FlagHandle){ .handle = handleR, .flag = "-R" };
	handles[2] = (FlagHandle){ .handle = handleU, .flag = "-U" };
	handles[3] = (FlagHandle){ .handle = handleN, .flag = "-N" };
	handles[4] = (FlagHandle){ .handle = handleC, .flag = "-C" };
	/* End of handle array */
	
	CMD* command = initCMD( argc, argv );
	if( !command ) return error( OUT_OF_MEM_ERR, NULL );
	
	int code = processCMD( command, handles, handlec, NULL );
	
	freeCMD( command );
	
	if( code ) return error( code, NULL );
}


