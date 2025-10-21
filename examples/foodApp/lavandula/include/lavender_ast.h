#ifndef LAVENDER_AST_H
#define LAVENDER_AST_H

typedef enum {
    SCHEMA_NODE_MODEL,
} SchemaNodeType;

typedef struct {
    char *name;
    char *type;
} ColumnDefinition;

typedef struct {
    char *name;

    ColumnDefinition *columns;
    int columnCount;
    int columnCapacity;
} ModelDeclaration;

typedef struct {
    SchemaNodeType type;

    union {
        ModelDeclaration model;
    };

} SchemaNode;

void freeSchemaNode(SchemaNode *node);
void printSchemaNode(const SchemaNode *node);

#endif