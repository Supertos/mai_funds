// -=-=-=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-=-=-
// Author: Supertos
//
// Memory Manager provides *some* abstraction over dynamic memory.
// Due to how the Memory Manager works, it is imperative that any allocated struct has 
// pointer to it's free method.
//
// e.g.
// struct Sample {
// void (*free)(void*);
// *other fields*
// };
// =-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-

#include "memman.h"
#include <stdio.h>
/* Initializes empty Memory Manager */
MemoryManager* initmemman() {
	MemoryManager* out = calloc( 1, sizeof( struct MemoryManager ) );
	out->entries = calloc( 1, sizeof( void* ) );
	out->capacity = 1;
	return out;
}

/* Frees Memory Manager as well as all it's elements */
void flushmemman( MemoryManager* memman ) {
	printf( "Initiating cleanup...\n" );
	for( size_t n = 0; n < memman->allocatedElements; n++ ) {
		printf( "Calling cleanup at %p for %p (%zu)...", *((void(**))memman->entries[n]), memman->entries[n], n );
		(*((void (**)(void*))memman->entries[n]))( memman->entries[n] ); // Cast void* to func pointer and call.
		printf( "DONE!\n" );
	}
	free( memman );
	
}

/* Makes memman bigger. Returns 0 on success */
unsigned int enlargememman( MemoryManager* memman ) {
	void* newEntries = realloc( memman->entries, memman->capacity * 2 );
	if( !newEntries ) return 1;
	
	memman->capacity *= 2;
	memman->entries = newEntries;
	return 0;
};

/* Registers new element in Memory Manager. Frees [element] on fail. */
void* registermemman( MemoryManager* memman, void* element ) {
	if( memman->capacity == memman->allocatedElements ) {
		if( enlargememman( memman ) ) {
			((void (*)(void*))element)( element );
			return NULL;
		}
	}
	memman->entries[memman->allocatedElements++] = element;
	return element;
}

