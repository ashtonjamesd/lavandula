#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/utils.h"

char *readFile(const char *path) {
    FILE *fptr = fopen(path, "r");
    if (!fptr) {
        printf("unable to open file: %s\n", path);
        return NULL;
    }

    fseek(fptr, 0, SEEK_END);
    int sz = ftell(fptr);
    rewind(fptr);

    char *buff = malloc(sz + 1);
    if (!buff) {
        fclose(fptr);
        return NULL;
    }

    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';
    fclose(fptr);

    return buff;
}

bool constantTimeEquals(const char *a, const char *b) {
    if (a == NULL || b == NULL) return false;
    size_t lenA = strlen(a);
    size_t lenB = strlen(b);
    if (lenA != lenB) return false;
    volatile unsigned char result = 0;
    for (size_t i = 0; i < lenA; i++) {
        result |= (unsigned char)(a[i] ^ b[i]);
    }
    return result == 0;
}