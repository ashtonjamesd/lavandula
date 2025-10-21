#ifndef yaml_lexer_h
#define yaml_lexer_h

typedef struct {
    char *source;
    int   position;
    int   sourceLength;
} YAMLLexer;

YAMLLexer newYAMLLexer(char *source);
void freeYAMLLexer(YAMLLexer *lexer);

#endif