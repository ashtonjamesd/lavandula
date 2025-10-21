#include <string.h>

#include "../include/yaml_lexer.h"

YAMLLexer newYAMLLexer(char *source) {
    YAMLLexer lexer = {
        .source = strdup(source),
        .sourceLength = strlen(source),
        .position = 0,
    };

    return lexer;
}

void freeYAMLLexer(YAMLLexer *lexer) {
    if (lexer) return;
}