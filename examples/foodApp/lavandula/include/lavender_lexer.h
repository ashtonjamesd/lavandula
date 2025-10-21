#ifndef LAVENDER_LEXER_H
#define LAVENDER_LEXER_H

#include <stdbool.h>

typedef enum {
    SCHEMA_TOKEN_ERR,

    SCHEMA_TOKEN_MODEL,
    SCHEMA_TOKEN_IDENTIFIER,
    SCHEMA_TOKEN_LBRACE,
    SCHEMA_TOKEN_RBRACE,
    SCHEMA_TOKEN_AT,
    SCHEMA_TOKEN_LPAREN,
    SCHEMA_TOKEN_RPAREN,
    SCHEMA_TOKEN_NUMBER,

    SCHEMA_TOKEN_INTEGER,
    SCHEMA_TOKEN_STRING,
    SCHEMA_TOKEN_BOOLEAN,
} SchemaTokenType;

typedef struct {
    SchemaTokenType type;
    char           *lexeme;
} SchemaToken;

typedef struct {
    const char *source;
    int         position;

    SchemaToken *tokens;
    int          tokenCount;
    int          tokenCapacity;

    bool        hadError;
} LavenderLexer;

LavenderLexer newLexer(const char *source);
void tokenizeSchema(LavenderLexer *lexer);
void freeLexer(LavenderLexer *lexer);

SchemaToken newTokenWithSize(const char *lexeme, int size, SchemaTokenType type);
SchemaToken newToken(const char *lexeme, SchemaTokenType type);
void freeToken(SchemaToken *token);
void printToken(const SchemaToken *token);

#endif