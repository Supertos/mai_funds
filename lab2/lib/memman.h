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

#pragma once
#include <stdlib.h>

typedef struct MemoryManager MemoryManager;
struct MemoryManager {
	size_t allocatedElements;		// How many elements had been allocated by code
	size_t capacity;				// How many element pointers can
	
	void** entries;					// Array of allocated structs with first members be the free function.
};

/* Initializes empty Memory Manager */
MemoryManager* initmemman();

/* Frees Memory Manager as well as all it's elements */
void flushmemman( MemoryManager* memman );

/* Makes memman bigger. Returns 0 on success */
unsigned int enlargememman( MemoryManager* memman );

/* Registers new element in Memory Manager */
void* registermemman( MemoryManager* memman, void* element );
