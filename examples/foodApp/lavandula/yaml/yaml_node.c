#include "../include/yaml_node.h"

YAMLNode newYAMLNode() {
    YAMLNode node = {};

    return node;
}

void freeYAMLNode(YAMLNode *node) {
    if (!node) return;
}