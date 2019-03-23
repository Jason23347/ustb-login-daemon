#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#define DEBUG true

#if (DEBUG == true)

#define DEBUG_BUFFER_MAX_SIZE 0x00ff

#define debug(fmt, ...) __debug(stdout, fmt, ##__VA_ARGS__)
#define debug_r(file, fmt, ...) __debug(file, fmt, ##__VA_ARGS__)

__fortify_function int
__debug (FILE *__restrict__ __stream, const char *__fmt, ...)
{
	return fprintf(__stream, "\033[41mDEBUG:\033[0m ") +
			fprintf(__stream, __fmt, __va_arg_pack ()) +
			fprintf(__stream, "\n");
}

#else

#define debug(fmt, ...)
#define debug_r(file, fmt, ...)
#endif

#endif