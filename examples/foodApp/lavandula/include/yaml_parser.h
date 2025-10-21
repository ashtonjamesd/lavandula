#ifndef yaml_parser_h
#define yaml_parser_h

typedef struct {
    int position;
} YAMLParser;

YAMLParser newYAMLParser();
void freeYAMLParser(YAMLParser *parser);

#endif