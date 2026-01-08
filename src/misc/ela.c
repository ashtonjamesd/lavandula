#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "include/ela.h"

ElaParser newElaParser(const char *source) {
    ElaParser parser = {
        .source = strdup(source),
    };

    if (!parser.source) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return parser;
}

void freeElaParser(ElaParser *parser) {
    if (!parser) return;

    free((char *)parser->source);
    parser->source = NULL;
}