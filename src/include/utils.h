#ifndef utils_h
#define utils_h

#include <stdbool.h>

char *readFile(const char *filename);
bool constantTimeEquals(const char *a, const char *b);

#endif