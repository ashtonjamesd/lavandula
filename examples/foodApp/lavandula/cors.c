#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/cors.h"

#define ORIGIN_LIMIT 50
#define HEADER_LIMIT 50

CorsConfig corsPolicy(void) {
    CorsConfig config = {
        .allowOrigin = malloc(ORIGIN_LIMIT * sizeof(char*)),
        .allowOriginCount = 0,
        .allowMethods = {0},
        .methodCount = 0,
        .allowHeaders = malloc(HEADER_LIMIT * sizeof(char*)),
        .headerCount = 0,
    };

    if (!config.allowOrigin || !config.allowHeaders) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return config;
}

void freeCorsPolicy(CorsConfig config) {
    for (int i = 0; i < config.allowOriginCount; i++) {
        free(config.allowOrigin[i]);
    }
    free(config.allowOrigin);

    for (int i = 0; i < config.headerCount; i++) {
        free(config.allowHeaders[i]);
    }
    free(config.allowHeaders);
}

void allowOrigin(CorsConfig *const config, const char *const origin) {
    if (config->allowOriginCount >= ORIGIN_LIMIT) {
        return;
    }

    config->allowOrigin[config->allowOriginCount++] = strdup(origin);
}

void allowMethod(CorsConfig *const config, HttpMethod method) {
    if (config->methodCount < 6) {
        config->allowMethods[config->methodCount++] = method;
    }
}

void allowAnyOrigin(CorsConfig *const config) {
    allowOrigin(config, "*");
}

void allowAnyMethod(CorsConfig *const config) {
    for (int i = HTTP_GET; i <= HTTP_OPTIONS; i++) {
        allowMethod(config, i);
    }
}

void allowHeader(CorsConfig *const config, const char *const header) {
    if (config->headerCount >= HEADER_LIMIT) {
        return;
    }

    config->allowHeaders[config->headerCount++] = strdup(header);
}

void allowAnyHeader(CorsConfig *const config) {
    allowHeader(config, "*");
}

CorsConfig corsAllowAll(void) {
    CorsConfig config = corsPolicy();
    allowAnyOrigin(&config);
    allowAnyMethod(&config);

    return config;
}