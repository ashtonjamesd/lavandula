#ifndef lavender_transpiler_h
#define lavender_transpiler_h

#include "lavender_ast.h"

typedef struct {
    SchemaNode   *nodes;
    int           nodeCount;
} LavenderTranspiler;

LavenderTranspiler newTranspiler(SchemaNode *nodes, int nodeCount);
void transpileSchema(const SchemaNode *nodes, int nodeCount);

#endif