#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/include/lavandula_test.h"
#include "../src/include/cors.h"

void testCorsPolicyInit() {
    CorsConfig config = corsPolicy();

    expectNotNull(config.allowOrigin);
    expect(config.allowOriginCount, toBe(0));
    expect(config.allowMethods[0], toBe(0));
    expect(config.methodCount, toBe(0));
    expectNotNull(config.allowHeaders);
    expect(config.headerCount, toBe(0));

    freeCorsPolicy(config);
}

void testAllowOrigin() {
    CorsConfig config = corsPolicy();
    allowOrigin(&config, "https://example.com");
    expect(config.allowOriginCount, toBe(1));
    expect(strcmp(config.allowOrigin[0], "https://example.com"), toBe(0));

    allowOrigin(&config, "https://test.com");
    expect(config.allowOriginCount, toBe(2));
    expect(strcmp(config.allowOrigin[1], "https://test.com"), toBe(0));

    freeCorsPolicy(config);
}

void testAllowOriginLimit() {
    CorsConfig config = corsPolicy();

    // Add 50 origins (the limit)
    for (int i = 0; i < 50; i++) {
        allowOrigin(&config, "https://example.com");
    }
    expect(config.allowOriginCount, toBe(50));

    // Try to add one more
    allowOrigin(&config, "https://overflow.com");
    expect(config.allowOriginCount, toBe(50));

    freeCorsPolicy(config);
}

void testAllowMethod() {
    CorsConfig config = corsPolicy();

    allowMethod(&config, HTTP_GET);
    expect(config.methodCount, toBe(1));
    expect(config.allowMethods[0], toBe(HTTP_GET));

    allowMethod(&config, HTTP_POST);
    expect(config.methodCount, toBe(2));
    expect(config.allowMethods[1], toBe(HTTP_POST));

    freeCorsPolicy(config);
}

void testAllowMethodLimit() {
    CorsConfig config = corsPolicy();

    // Add all 6 methods (GET, POST, PUT, DELETE, PATCH, OPTIONS)
    for (int i = 0; i < 6; i++) {
        allowMethod(&config, i);
    }
    expect(config.methodCount, toBe(6));

    // Try to add one more
    allowMethod(&config, (HttpMethod)0xffff);
    expect(config.methodCount, toBe(6));

    freeCorsPolicy(config);
}

void testAllowAnyOrigin() {
    CorsConfig config = corsPolicy();

    allowAnyOrigin(&config);
    expect(config.allowOriginCount, toBe(1));
    expect(strcmp(config.allowOrigin[0], "*"), toBe(0));

    freeCorsPolicy(config);
}

void testAllowAnyMethod() {
    CorsConfig config = corsPolicy();

    allowAnyMethod(&config);
    expect(config.methodCount, toBe(6));

    for (int i = 0; i < 6; i++) {
        expect(config.allowMethods[i], toBe((HttpMethod)i));
    }

    freeCorsPolicy(config);
}

void testAllowHeader() {
    CorsConfig config = corsPolicy();

    allowHeader(&config, "Content-Type");
    expect(config.headerCount, toBe(1));
    expect(strcmp(config.allowHeaders[0], "Content-Type"), toBe(0));

    allowHeader(&config, "Authorization");
    expect(config.headerCount, toBe(2));
    expect(strcmp(config.allowHeaders[1], "Authorization"), toBe(0));

    freeCorsPolicy(config);
}

void testAllowHeaderLimit() {
    CorsConfig config = corsPolicy();

    // Add 50 headers (the limit)
    for (int i = 0; i < 50; i++) {
        allowHeader(&config, "X-Custom-Header");
    }
    expect(config.headerCount, toBe(50));

    // Try to add one more
    allowHeader(&config, "X-Overflow");
    expect(config.headerCount, toBe(50));

    freeCorsPolicy(config);
}

void testAllowAnyHeader() {
    CorsConfig config = corsPolicy();

    allowAnyHeader(&config);
    expect(config.headerCount, toBe(1));
    expect(strcmp(config.allowHeaders[0], "*"), toBe(0));

    freeCorsPolicy(config);
}

void testCorsAllowAll() {
    CorsConfig config = corsAllowAll();

    // Check that origin is set to "*"
    expect(config.allowOriginCount, toBe(1));
    expect(strcmp(config.allowOrigin[0], "*"), toBe(0));

    // Check that all methods are allowed
    expect(config.methodCount, toBe(6));
    for (int i = 0; i < 6; i++) {
        expect(config.allowMethods[i], toBe((HttpMethod)i));
    }
    freeCorsPolicy(config);
}

void testAllowOriginWithDynamicStrings() {
    CorsConfig config = corsPolicy();

    char *dynamic_origin = strdup("https://dynamic.com");
    allowOrigin(&config, dynamic_origin);
    free(dynamic_origin);
    expect(config.allowOriginCount, toBe(1));
    expect(strcmp(config.allowOrigin[0], "https://dynamic.com"), toBe(0));

    allowOrigin(&config, "https://literal.com");
    expect(config.allowOriginCount, toBe(2));
    expect(strcmp(config.allowOrigin[1], "https://literal.com"), toBe(0));

    freeCorsPolicy(config);
}

void testAllowHeaderWithDynamicStrings() {
    CorsConfig config = corsPolicy();

    char *dynamic_header = strdup("X-Custom-Header");
    allowHeader(&config, dynamic_header);
    free(dynamic_header);
    expect(config.headerCount, toBe(1));
    expect(strcmp(config.allowHeaders[0], "X-Custom-Header"), toBe(0));

    allowHeader(&config, "Authorization");
    expect(config.headerCount, toBe(2));
    expect(strcmp(config.allowHeaders[1], "Authorization"), toBe(0));

    freeCorsPolicy(config);
}

void runCorsTests() {
    runTest(testCorsPolicyInit);
    runTest(testAllowOrigin);
    runTest(testAllowOriginLimit);
    runTest(testAllowMethod);
    runTest(testAllowMethodLimit);
    runTest(testAllowAnyOrigin);
    runTest(testAllowAnyMethod);
    runTest(testAllowHeader);
    runTest(testAllowHeaderLimit);
    runTest(testAllowAnyHeader);
    runTest(testCorsAllowAll);
    runTest(testAllowOriginWithDynamicStrings);
    runTest(testAllowHeaderWithDynamicStrings);
}