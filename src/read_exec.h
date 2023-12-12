#ifndef __READ_EXEC_H__
#define __READ_EXEC_H__

#include "assembly.h"
#include "memory.h"

#include <stdio.h>

// read file into simulated memory, return value of _start symbol
int read_exec(struct memory*, struct assembly*, const char*, FILE* log_file);

#endif
