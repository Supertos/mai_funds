// -=-=-=-=-=-=-=-=-=-= Supertos Industries ( 2012 - 2024 ) =-=-=-=-=-=-=-=-=-=-
// Author: Supertos
// 
// Vector implementation with realloc and shit.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#pragma once
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#define max(a, b) a > b ? a : b

#define DEFINE_VECTOR(vectortype, vectorname)																\
typedef struct vectorname vectorname;																		\
struct vectorname {																							\
	void (*free)(void*);																					\
	size_t length;																							\
	size_t capacity;																						\
	vectortype* data;																						\
};																											\
																											\
void delete##vectorname( void* obj ) {																		\
	free( ((vectorname*)obj)->data );																		\
	free( obj );																							\
}																											\
vectorname* new##vectorname() {																				\
	vectorname* obj = malloc( sizeof(vectorname) );															\
	if( !obj ) return NULL;																					\
	*obj = (vectorname){ .length = 0, .capacity = 0 };														\
	obj->free = delete##vectorname;																			\
	return obj;																								\
}																											\
																											\
																											\
vectorname* resize##vectorname( vectorname* obj, size_t size ) {											\
	vectortype* newobj = realloc( obj->data, size * sizeof(vectortype) );									\
	if( !newobj ) return NULL;																				\
	obj->capacity = size;																					\
	obj->data = newobj;																						\
	if( obj->capacity < obj->length ) obj->length = obj->capacity;											\
	return obj;/* Would trim array to size. */																\
}																											\
																											\
vectorname* push##vectorname( vectorname* obj, vectortype element ) {										\
	if( obj->length >= obj->capacity ) 																		\
		if( !resize##vectorname( obj, (obj->capacity+1) * 2 ) ) return NULL;								\
	obj->data[obj->length++] = element;																		\
	return obj;																								\
}																											\
																											\
vectortype* get##vectorname( vectorname* obj, size_t pos ) { 												\
	if( pos >= obj->length ) return NULL;																	\
	return &obj->data[pos];																					\
}																											\
																											\
vectortype* set##vectorname( vectorname* obj, vectortype element, size_t id ) {								\
	if( id >= obj->length ) return NULL; /* obj->capacity behaivour inconsistient */						\
	obj->data[id] = element;																				\
	return &obj->data[id];																					\
}																											\
																											\
vectortype* last##vectorname( vectorname* obj ) {															\
	return &obj->data[obj->length - 1];																		\
}																											\
vectorname* concat##vectorname( vectorname* a, vectorname* b ) {											\
	if( !resize##vectorname( a, a->length + b->length ) ) return NULL;										\
	for( size_t n = 0; n < b->length; n++ )																	\
		push##vectorname( a, b->data[n] );																	\
	return a;																								\
}																											\
																											\
vectortype shift##vectorname( vectorname* obj, size_t el ) {												\
	vectortype shift = obj->data[el];																		\
	for( size_t n = el; n < obj->length- 1; n++ ) {															\
		obj->data[n] = obj->data[n+1];																		\
	}																										\
	obj->length--;																							\
																											\
	return shift;																							\
}																											\
																											\
vectorname* pusharr##vectorname( vectorname* obj, vectortype* arr, size_t n ) {								\
	for( size_t a = 0; a < n; a++ ) 																		\
		if( !push##vectorname( obj, arr[a] ) ) return NULL;													\
	return obj;																								\
}																											\
																											\
vectorname* resizefront##vectorname( vectorname* obj, size_t len ) { /* TODO: Implement shrink */			\
	if( len <= obj->length ) return NULL;																	\
	size_t oldlen = obj->length;																			\
																											\
	if( !resize##vectorname( obj, len ) ) return NULL;														\
	obj->length = obj->capacity;																			\
	memmove( &obj->data[len - oldlen], obj->data, oldlen * sizeof(vectortype) );							\
	memset( obj->data, '\0', (len - oldlen) * sizeof(vectortype) );											\
	return obj;																								\
};																											\
	
	


