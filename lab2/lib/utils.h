// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Supertos Industries ( 2012 - 2024 )
// Author: Supertos
// Command line utilities
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <stdio.h>
#include "memman.h"

#define NO_ERR 0
#define INVALID_ARGUMENT_ORDER_ERR 1
#define OUT_OF_MEM_ERR 2
#define NO_SUCH_FILE_ERR 4
#define READ_ERR 8
#define WRITE_ERR 16
#define INVALID_FLAG_ERR 32
#define INVALID_INPUT_ERR 64
#define FILENAME_MATCH_ERR 128

#define RANDOM(a, b) (int)(a + ((double)rand() / (double)RAND_MAX) * (b - a))

/* Prints custom error [text], flushes [man] and then returns [code] */
int error( int code, MemoryManager* man );

/* Flushes [man] and returns [code] */
int serror( int code, MemoryManager* man );