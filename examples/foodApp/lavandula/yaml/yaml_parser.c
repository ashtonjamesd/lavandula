#include "../include/yaml_parser.h"

YAMLParser newYAMLParser() {
    YAMLParser parser = {
        .position = 0
    };

    return parser;
}
void freeYAMLParser(YAMLParser *parser) {
    if (!parser) return;
}