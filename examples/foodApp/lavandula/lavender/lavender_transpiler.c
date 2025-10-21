#include <stdio.h>
#include <string.h>

#include "../include/lavender_transpiler.h"
#include "../include/lavender.h"

LavenderTranspiler newTranspiler(SchemaNode *nodes, int nodeCount) {
    LavenderTranspiler transpiler = {
        .nodes = nodes,
        .nodeCount = nodeCount,
    };

    return transpiler;
}

static const char *sqlTypeForSchemaType(char *columnType) {
    if (strcmp(columnType, "String") == 0) {
        return "TEXT";
    } else if (strcmp(columnType, "Integer") == 0) {
        return "INTEGER";
    } else if (strcmp(columnType, "Boolean") == 0) {
        return "BOOLEAN";
    }
    
    return "TEXT";
}

void transpileModelDeclaration(const ModelDeclaration *model) {
    FILE *fptr = fopen("output.sql", "w");
    if (!fptr) {
        printf("Error opening output.sql for writing\n");
        return;
    }
    
    fprintf(fptr, "CREATE TABLE %s (\n", model->name);
    for (int i = 0; i < model->columnCount; i++) {
        ColumnDefinition *col = &model->columns[i];
        fprintf(fptr, "    %s %s", col->name, sqlTypeForSchemaType(col->type));
        if (i < model->columnCount - 1) {
            fprintf(fptr, ",");
        }
        fprintf(fptr, "\n");
    }
    fprintf(fptr, ");\n");
}

void transpileSchema(const SchemaNode *nodes, int nodeCount) {
    for (int i = 0; i < nodeCount; i++) {
        const SchemaNode *node = &nodes[i];
        if (node->type == SCHEMA_NODE_MODEL) {
            transpileModelDeclaration(&node->model);
        } else {
            printf("Unknown schema node type: %d\n", node->type);
        }
    }
}