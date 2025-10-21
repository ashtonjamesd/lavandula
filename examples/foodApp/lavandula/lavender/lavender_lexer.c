#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "../include/lavender_lexer.h"
#include "../include/utils.h"

static bool isEnd(LavenderLexer *lexer) {
    return lexer->position >= (int)strlen(lexer->source);
}

static void advance(LavenderLexer *lexer) {
    lexer->position++;
}

static char current(LavenderLexer *lexer) {
    return lexer->source[lexer->position];
}

SchemaToken newTokenWithSize(const char *lexeme, int size, SchemaTokenType type) {
    return (SchemaToken) {
        .lexeme = strndup(lexeme, size),
        .type = type,
    };
}

SchemaToken newToken(const char *lexeme, SchemaTokenType type) {
    return (SchemaToken) {
        .lexeme = strdup(lexeme),
        .type = type,
    };
}

void freeToken(SchemaToken *token) {
    if (token && token->lexeme) {
        free(token->lexeme);
        token->lexeme = NULL;
    }
}

void printToken(const SchemaToken *token) {
    if (token) {
        printf("Token: Type=%d, Lexeme='%s'\n", token->type, token->lexeme);
    }
}

static bool isValidIdentifierChar(char c) {
    return isalnum(c) || c == '_';
}

static void skipWhitespace(LavenderLexer *lexer) {
    while (!isEnd(lexer) && (isspace(current(lexer)) || current(lexer) == '\r')) {
        advance(lexer);
    }
}

static SchemaToken parseSymbol(LavenderLexer *lexer) {
    char c = current(lexer);
    advance(lexer);

    switch (c) {
        case '{':
            return newToken("{", SCHEMA_TOKEN_LBRACE);
        case '}':
            return newToken("}", SCHEMA_TOKEN_RBRACE);
        case '@':
            return newToken("@", SCHEMA_TOKEN_AT);
        case '(':
            return newToken("(", SCHEMA_TOKEN_LPAREN);
        case ')':
            return newToken(")", SCHEMA_TOKEN_RPAREN);
    }

    return newToken("", SCHEMA_TOKEN_ERR);
}

static SchemaToken parseIdentifier(LavenderLexer *lexer) {
    int start = lexer->position;

    while (!isEnd(lexer) && isValidIdentifierChar(current(lexer))) {
        advance(lexer);
    }

    int length = lexer->position - start;
    const char *lexeme = lexer->source + start;

    SchemaTokenType type = SCHEMA_TOKEN_IDENTIFIER;
    if (strncmp(lexeme, "model", length) == 0) {
        type = SCHEMA_TOKEN_MODEL;
    } else if (strncmp(lexeme, "String", length) == 0) {
        type = SCHEMA_TOKEN_STRING;
    } else if (strncmp(lexeme, "Integer", length) == 0) {
        type = SCHEMA_TOKEN_INTEGER;
    } else if (strncmp(lexeme, "Boolean", length) == 0) {
        type = SCHEMA_TOKEN_BOOLEAN;
    }

    return newTokenWithSize(lexeme, length, type);
}

static SchemaToken parseNumber(LavenderLexer *lexer) {
    int start = lexer->position;

    while (!isEnd(lexer) && isdigit(current(lexer))) {
        advance(lexer);
    }

    int length = lexer->position - start;
    const char *lexeme = lexer->source + start;
    return newTokenWithSize(lexeme, length, SCHEMA_TOKEN_NUMBER);
}

static SchemaToken tryParse(LavenderLexer *lexer) {
    skipWhitespace(lexer);
    if (isEnd(lexer)) return newToken("", SCHEMA_TOKEN_ERR);

    char c = current(lexer);

    if (c == '{' || c == '}' || c == '@' || c == '(' || c == ')') {
        return parseSymbol(lexer);
    }

    if (isdigit(c)) {
        return parseNumber(lexer);
    }

    if (isValidIdentifierChar(c)) {
        SchemaToken token = parseIdentifier(lexer);
        return token;
    }

    advance(lexer);
    return newToken("", SCHEMA_TOKEN_ERR);
}

void tokenizeSchema(LavenderLexer *lexer) {
    char *source = readFile(lexer->source);
    if (!source) return;

    lexer->source = source;

    while (!isEnd(lexer)) {
        skipWhitespace(lexer);
        if (isEnd(lexer)) break;

        SchemaToken token = tryParse(lexer);

        if (lexer->tokenCount >= lexer->tokenCapacity) {
            lexer->tokenCapacity *= 2;
            lexer->tokens = realloc(lexer->tokens, sizeof(SchemaToken) * lexer->tokenCapacity);

            if (!lexer->tokens) {
                fprintf(stderr, "Fatal: out of memory\n");
                exit(EXIT_FAILURE);
            }
        }

        lexer->tokens[lexer->tokenCount++] = token;

        if (token.type == SCHEMA_TOKEN_ERR) {
            lexer->hadError = true;
            break;
        }
    }

    free(source);
}

LavenderLexer newLexer(const char *source) {
    LavenderLexer lexer = {
        .source = source,
        .position = 0,
        .tokens = malloc(sizeof(SchemaToken)),
        .tokenCount = 0,
        .tokenCapacity = 1,
        .hadError = false,
    };

    if (!lexer.tokens) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return lexer;
}

void freeLexer(LavenderLexer *lexer) {
    if (!lexer) return;
    
    for (int i = 0; i < lexer->tokenCount; i++) {
        freeToken(&lexer->tokens[i]);
    }

    free(lexer->tokens);
    lexer->tokens = NULL;
    lexer->tokenCount = 0;
    lexer->tokenCapacity = 0;
}