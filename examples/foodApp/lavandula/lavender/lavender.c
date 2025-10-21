#include <stdlib.h>
#include <stdio.h>

#include "../include/lavender.h"
#include "../include/lavender_parser.h"

LavenderSchemaParser newSchemaParser(const char *source) {
    LavenderLexer *lexer = malloc(sizeof(LavenderLexer));
    if (!lexer) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    *lexer = newLexer(source);
    
    LavenderParser *parser = malloc(sizeof(LavenderParser));
    if (!parser) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    *parser = newParser(lexer);

    LavenderTranspiler *transpiler = malloc(sizeof(LavenderTranspiler));
    if (!transpiler) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    *transpiler = newTranspiler(parser->nodes, parser->nodeCount);

    LavenderSchemaParser schemaParser = {
        .source = source,
        .lexer = lexer,
        .parser = parser,
        .transpiler = transpiler,
        .hadError = false,
    };

    return schemaParser;
}

void freeSchemaParser(LavenderSchemaParser *schemaParser) {
    if (!schemaParser) return;

    if (schemaParser->parser) {
        freeLavenderParser(schemaParser->parser);
        free(schemaParser->parser);
    }

    if (schemaParser->lexer) {
        freeLexer(schemaParser->lexer);
        free(schemaParser->lexer);
    }
}