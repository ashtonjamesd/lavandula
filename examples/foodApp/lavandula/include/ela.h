#ifndef ela_h
#define ela_h

typedef struct {
    const char *source;
} ElaParser;

ElaParser newElaParser(const char *source);
void freeElaParser(ElaParser *parser);

#endif