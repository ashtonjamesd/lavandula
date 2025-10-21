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
    size_t sz = ftell(fptr);
    rewind(fptr);

    char *buff = malloc(sz + 1);
    if (!buff) {
        fclose(fptr);
        return NULL;
    }

    if (fread(buff, 1, sz, fptr) != sz) {
        fclose(fptr);
        return NULL;
    }
    buff[sz] = '\0';
    fclose(fptr);

    return buff;
}