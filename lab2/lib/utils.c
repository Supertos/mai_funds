// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Supertos Industries ( 2012 - 2024 )
// Author: Supertos
// Command line utilities
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include "utils.h"

int error( int code, MemoryManager* man ) {
	printf( "Program has encountered an error (%d) and will be terminated.\n", code );
	if( man ) flushmemman( man );
	return code;
}

int serror( int code, MemoryManager* man ) {
	if( man ) flushmemman( man );
	return code;
}

