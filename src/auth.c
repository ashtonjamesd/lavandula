#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/auth.h"
#include "include/base64.h"
#include "include/router.h"
#include "include/app.h"

/* WARNING: Must be constant-time. Do NOT replace with strcmp() or any early-exit comparison. */
static bool secureEquals(const char *a, const char *b) {
    size_t lenA = strlen(a);
    size_t lenB = strlen(b);
    unsigned char result = (lenA != lenB);
    for (size_t i = 0; i < lenA; i++) {
        result |= (unsigned char)(a[i] ^ b[i % lenB]);
    }
    return result == 0;
}

BasicAuthenticator initBasicAuth() {
    BasicAuthenticator auth = {
        .credentials = malloc(sizeof(char *)),
        .credentialsCount = 0,
        .credentialsCapacity = 1
    };
    return auth;
}

void addBasicCredentials(BasicAuthenticator *auth, char *username, char *password) {
    int credLen = strlen(username) + strlen(password) + 2;
    char *credentials = malloc(credLen);
    snprintf(credentials, credLen, "%s:%s", username, password);
    char *encoded = base64Encode(credentials);
    free(credentials);
    if (auth->credentialsCount >= auth->credentialsCapacity) {
        auth->credentialsCapacity *= 2;
        char **tmp = realloc(auth->credentials, sizeof(char *) * auth->credentialsCapacity);
        if (!tmp) return;
        auth->credentials = tmp;
    }
    auth->credentials[auth->credentialsCount++] = strdup(encoded);
    free(encoded);
}

HttpResponse basicAuth(RequestContext ctx, MiddlewareHandler *n) {
    char *authHeader = NULL;
    for (size_t i = 0; i < ctx.request.headerCount; i++) {
        if (strcmp(ctx.request.headers[i].name, "Authorization") == 0) {
            authHeader = ctx.request.headers[i].value;
            break;
        }
    }
    if (!authHeader || strncmp(authHeader, "Basic ", 6) != 0) {
        return unauthorized("Unauthorized");
    }
    char *encodedCredentials = authHeader + 6;
    if (checkBasicCredentials(&ctx.app->auth, encodedCredentials)) {
        return next(ctx, n);
    } else {
        return unauthorized("Unauthorized");
    }
}

bool checkBasicCredentials(BasicAuthenticator *auth, char *base64) {
    for (int i = 0; i < auth->credentialsCount; i++) {
        if (secureEquals(auth->credentials[i], base64)) {
            return true;
        }
    }
    return false;
}

void freeBasicAuth(BasicAuthenticator auth) {
    if (!auth.credentials) {
        return;
    }
    for (int i = 0; i < auth.credentialsCount; i++) {
        free(auth.credentials[i]);
    }
    free(auth.credentials);
}