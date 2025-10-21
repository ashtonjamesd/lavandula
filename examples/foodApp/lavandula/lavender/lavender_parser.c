#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/lavender_parser.h"
#include "../include/lavender_transpiler.h"
#include "../include/lavender.h"

static bool isLast(LavenderParser *parser) {
    return parser->position >= parser->lexer->tokenCount;
}

static void advance(LavenderParser *parser) {
    parser->position++;
}

static SchemaToken currentToken(LavenderParser *parser) {
    if (isLast(parser)) {
        SchemaToken eofToken = {
            .type = SCHEMA_TOKEN_ERR,
            .lexeme = "",
        };

        return eofToken;
    }

    return parser->lexer->tokens[parser->position];
}

static SchemaNode badNode(LavenderParser *parser) {
    parser->hadError = true;

    SchemaNode node = {
        .type = -1,
    };

    return node;
}

static bool expectToken(LavenderParser *parser, SchemaTokenType type) {
    if (isLast(parser)) return false;

    if (currentToken(parser).type != type) return false;

    advance(parser);
    return true;
}

static bool match(LavenderParser *parser, SchemaTokenType type) {
    if (isLast(parser)) return false;
    if (currentToken(parser).type != type) return false;

    return true;
}

static bool isValidType(SchemaToken token) {
    return token.type == SCHEMA_TOKEN_STRING 
        || token.type == SCHEMA_TOKEN_INTEGER
        || token.type == SCHEMA_TOKEN_BOOLEAN;
}

static SchemaNode parseModel(LavenderParser *parser) {
    advance(parser);
    if (isLast(parser)) {
        return badNode(parser);
    }

    SchemaToken nameToken = parser->lexer->tokens[parser->position];
    if (nameToken.type != SCHEMA_TOKEN_IDENTIFIER) {
        printf("Expected model name after 'model', got: %s\n", nameToken.lexeme);
        return badNode(parser);
    }
    advance(parser);

    if (!expectToken(parser, SCHEMA_TOKEN_LBRACE)) {
        printf("Expected '{' after model name, got: %s\n", parser->lexer->tokens[parser->position].lexeme);
        return badNode(parser);
    }

    int columnCapacity = 1;
    int columnCount = 0;
    ColumnDefinition *columns = malloc(sizeof(ColumnDefinition));

    if (!columns) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    while (!match(parser, SCHEMA_TOKEN_RBRACE) && !isLast(parser)) {
        ColumnDefinition column;

        SchemaToken colNameToken = parser->lexer->tokens[parser->position];
        if (colNameToken.type != SCHEMA_TOKEN_IDENTIFIER) {
            printf("Expected column name in model, got: %s\n", colNameToken.lexeme);
            free(columns);
            return badNode(parser);
        }
        advance(parser);

        column.name = strdup(colNameToken.lexeme);

        SchemaToken typeToken = parser->lexer->tokens[parser->position];
        if (!isValidType(typeToken)) {
            printf("Expected column type in model, got: %s\n", typeToken.lexeme);
            free(columns);
            return badNode(parser);
        }
        advance(parser);

        column.type = strdup(typeToken.lexeme);

        if (columnCount >= columnCapacity) {
            columnCapacity *= 2;
            columns = realloc(columns, sizeof(ColumnDefinition) * columnCapacity);

            if (!columns) {
                fprintf(stderr, "Fatal: out of memory\n");
                exit(EXIT_FAILURE);
            }
        }
        columns[columnCount++] = column;

    }

    if (!expectToken(parser, SCHEMA_TOKEN_RBRACE)) {
        printf("Expected '}' after model name, got: %s\n", parser->lexer->tokens[parser->position].lexeme);
        return badNode(parser);
    }

    SchemaNode node = {
        .type = SCHEMA_NODE_MODEL,
        .model = {
            .name = strdup(nameToken.lexeme),
            .columns = columns,
            .columnCount = columnCount,
        },
    };

    return node;
}

static SchemaNode parseSchemaStatement(LavenderParser *parser) {
    SchemaToken token = parser->lexer->tokens[parser->position];

    switch (token.type) {
        case SCHEMA_TOKEN_MODEL:
            return parseModel(parser);
        default:
            parser->hadError = true;
            printf("Unexpected token: %s\n", token.lexeme);
            exit(1);
    }
}

static void createSchemaAst(LavenderParser *parser) {
    parser->position = 0;

    while (!isLast(parser)) {
        SchemaNode node = parseSchemaStatement(parser);
        if (parser->hadError) return;

        if (parser->nodeCount >= parser->nodeCapacity) {
            parser->nodeCapacity *= 2;
            parser->nodes = realloc(parser->nodes, sizeof(SchemaNode) * parser->nodeCapacity);

            if (!parser->nodes) {
                fprintf(stderr, "Fatal: out of memory\n");
                exit(EXIT_FAILURE);
            }
        }

        parser->nodes[parser->nodeCount++] = node;
    }
}

void parseSchema(LavenderSchemaParser *lavender) {
    tokenizeSchema(lavender->lexer);
    if (lavender->lexer->hadError) {
        lavender->hadError = true;
        return;
    }

    // for (int i = 0; i < parser->lexer->tokenCount; i++) {
    //     printToken(&parser->lexer->tokens[i]);
    // }

    createSchemaAst(lavender->parser);
    if (lavender->parser->hadError) return;

    for (int i = 0; i < lavender->parser->nodeCount; i++) {
        printSchemaNode(&lavender->parser->nodes[i]);
    }

    transpileSchema(lavender->parser->nodes, lavender->parser->nodeCount);
}

LavenderParser newParser(LavenderLexer *lexer) {
    LavenderParser parser = {
        .lexer = lexer,
        .position = 0,
        .nodes = malloc(sizeof(SchemaNode)),
        .nodeCount = 0,
        .nodeCapacity = 1,
        .hadError = false,
    };

    if (!parser.nodes) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return parser;
}

void freeLavenderParser(LavenderParser *parser) {
    if (!parser) return;

    for (int i = 0; i < parser->nodeCount; i++) {
        freeSchemaNode(&parser->nodes[i]);
    }

    free(parser->nodes);
    parser->nodes = NULL;
    parser->nodeCount = 0;
    parser->nodeCapacity = 0;
}