#include <stdlib.h>
#include <string.h>

#include "include/ela.h"

ElaParser newElaParser(const char *source) {
    ElaParser parser = {
        .source = strdup(source),
    };

    return parser;
}

void freeElaParser(ElaParser *parser) {
    if (!parser) return;

    free((char *)parser->source);
    parser->source = NULL;
}