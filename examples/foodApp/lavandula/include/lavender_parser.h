#ifndef LAVENDER_PARSER_H
#define LAVENDER_PARSER_H

#include "lavender_ast.h"
#include "lavender_lexer.h"
#include <stdbool.h>

typedef struct LavenderParser {
    LavenderLexer *lexer;
    int           position;

    SchemaNode   *nodes;
    int           nodeCount;
    int           nodeCapacity;

    bool          hadError;
} LavenderParser;

LavenderParser newParser(LavenderLexer *lexer);
void freeLavenderParser(LavenderParser *parser);

#endif