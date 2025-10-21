#ifndef yaml_node_h
#define yaml_node_h

typedef struct {
    int x;
} YAMLNode;

YAMLNode newYAMLNode();
void freeYAMLNode(YAMLNode *node);

#endif