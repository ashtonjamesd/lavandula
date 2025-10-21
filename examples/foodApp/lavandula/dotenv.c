#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "include/dotenv.h"
#include "include/utils.h"

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMERIC,
    TOKEN_EQUALS,
    TOKEN_ERROR,
} TokenType;

typedef struct {
    char     *lexeme;
    TokenType type;
} Token;

typedef struct {
    char  *source;
    int    position;

    Token *tokens;
    int    tokenCount;  
    int    tokenCapacity;  
} DotenvParser;

typedef struct {
    char *key;
    char *value;
} MapEntry;

typedef struct {
    MapEntry *entries;
    int       entryCount;
    int       entryCapacity;
} Map;

Map map;

char *env(char *variable) {
    for (int i = 0; i < map.entryCount; i++) {
        if (strcmp(map.entries[i].key, variable) == 0) {
            return map.entries[i].value;
        }
    }

    return NULL;
}

static bool isEnd(DotenvParser *p) {
    return p->position >= (int)strlen(p->source);
}

static void advance(DotenvParser *p) {
    p->position++;
}

static char current(DotenvParser *p) {
    return p->source[p->position];
}

static Token newToken(const char *lexeme, TokenType type) {
    Token token = {
        .lexeme = strdup(lexeme),
        .type = type,
    };
    return token;
}

static bool isValidIdentifierChar(char c) {
    return isalnum(c) || c == '_';
}

static void skipWhitespace(DotenvParser *p) {
    while (!isEnd(p) && (isspace(current(p)) || current(p) == '\r')) {
        advance(p);
    }
}

static Token parseSymbol(DotenvParser *p) {
    char c = current(p);
    advance(p);

    if (c == '=') {
        return newToken("=", TOKEN_EQUALS);
    }

    return newToken("", TOKEN_ERROR);
}

static Token parseIdentifier(DotenvParser *p) {
    int start = p->position;

    while (!isEnd(p) && isValidIdentifierChar(current(p))) {
        advance(p);
    }

    int length = p->position - start;
    char *lexeme = strndup(p->source + start, length);
    return newToken(lexeme, TOKEN_IDENTIFIER);
}

Token parseString(DotenvParser *p) {
    advance(p);
    int start = p->position;

    while (!isEnd(p) && current(p) != '"') {
        advance(p);
    }

    int length = p->position - start;
    char *lexeme = strndup(p->source + start, length);

    if (current(p) == '"') {
        advance(p);
    }

    return newToken(lexeme, TOKEN_STRING);
}

Token parseValue(DotenvParser *p) {
    if (current(p) == '"') {
        return parseString(p);
    }

    int start = p->position;
    while (!isEnd(p) && !isspace(current(p))) {
        advance(p);
    }

    int length = p->position - start;
    char *lexeme = strndup(p->source + start, length);

    bool isNumeric = true;
    for (int i = 0; i < length; i++) {
        if (!isdigit(lexeme[i])) {
            isNumeric = false;
            break;
        }
    }

    return newToken(lexeme, isNumeric ? TOKEN_NUMERIC : TOKEN_STRING);
}

static Token tryParse(DotenvParser *p) {
    skipWhitespace(p);

    if (isEnd(p)) return newToken("", TOKEN_ERROR);

    char c = current(p);
    if (isValidIdentifierChar(c)) {
        return parseIdentifier(p);
    }

    if (c == '=') {
        return parseSymbol(p);
    }

    if (c == '"' || isdigit(c)) {
        return parseValue(p);
    }

    return newToken("", TOKEN_ERROR);
}

static void parse(DotenvParser *p) {
    while (!isEnd(p)) {
        Token token = tryParse(p);

        if (p->tokenCount >= p->tokenCapacity) {
            p->tokenCapacity *= 2;
            p->tokens = realloc(p->tokens, sizeof(Token) * p->tokenCapacity);
        }

        if (!p->tokens) {
            fprintf(stderr, "Fatal: out of memory\n");
            exit(EXIT_FAILURE);
        }

        p->tokens[p->tokenCount++] = token;

        if (token.type == TOKEN_ERROR) {
            break;
        }
    }
}

void dotenvClean() {
    for (int i = 0; i < map.entryCount; i++) {
        free(map.entries[i].key);
        free(map.entries[i].value);
    }

    free(map.entries);
}

void dotenv(char *path) {
    char *source = readFile(path);
    if (!source) return;

    map = (Map){
        .entries = malloc(sizeof(MapEntry)),
        .entryCapacity = 1,
        .entryCount = 0
    };

    if (!map.entries) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    DotenvParser p = {
        .position = 0,
        .source = source,
        .tokenCapacity = 4,
        .tokenCount = 0,
        .tokens = malloc(sizeof(Token) * 4)
    };

    if (!p.tokens) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    parse(&p);

    // for (int i = 0; i < p.tokenCount; i++) {
    //     printf("[%s] : %d\n", p.tokens[i].lexeme, p.tokens[i].type);
    // }

    for (int i = 0; i < p.tokenCount; i++) {

        Token keyToken = p.tokens[i++];
        if (i >= p.tokenCount) break;

        if (keyToken.type != TOKEN_IDENTIFIER) break;

        Token equalsToken = p.tokens[i++];
        if (i >= p.tokenCount) break;
        if (equalsToken.type != TOKEN_EQUALS) break;

        Token valueToken = p.tokens[i];
        if (i >= p.tokenCount) break;

        
        MapEntry entry = {
            .key = strdup(keyToken.lexeme),
            .value = strdup(valueToken.lexeme)
        };

        if (map.entryCount >= map.entryCapacity) {
            map.entryCapacity *= 2;
            map.entries = realloc(map.entries, sizeof(MapEntry) * map.entryCapacity);
        }
        
        if (!map.entries) {
            fprintf(stderr, "Fatal: out of memory\n");
            exit(EXIT_FAILURE);
        }

        map.entries[map.entryCount++] = entry;
    }


    for (int i = 0; i < p.tokenCount; i++) {
        free(p.tokens[i].lexeme);
    }
    free(p.tokens);
    free(source);
}