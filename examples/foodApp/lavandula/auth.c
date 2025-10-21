#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/auth.h"
#include "include/base64.h"
#include "include/router.h"
#include "include/app.h"

BasicAuthenticator initBasicAuth(void) {
    BasicAuthenticator auth = {
        .credentials = malloc(sizeof(char *)),
        .credentialsCount = 0,
        .credentialsCapacity = 1
    };

    if (!auth.credentials) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return auth;
}

void addBasicCredentials(BasicAuthenticator *auth, const char *const username, const char *const password) {
    int credLen = strlen(username) + strlen(password) + 2;
    char *credentials = malloc(credLen);

    if (!credentials) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    snprintf(credentials, credLen, "%s:%s", username, password);
    
    char *encoded = base64Encode(credentials);
    free(credentials);
    
    if (auth->credentialsCount >= auth->credentialsCapacity) {
        auth->credentialsCapacity *= 2;
        auth->credentials = realloc(auth->credentials, sizeof(char *) * auth->credentialsCapacity);
    }

    if (!auth->credentials) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    auth->credentials[auth->credentialsCount++] = encoded;
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
        return unauthorized("Unauthorized", TEXT_PLAIN);
    }

    char *encodedCredentials = authHeader + 6;

    if (checkBasicCredentials(&ctx.app->auth, encodedCredentials)) {
        return next(ctx, n);
    } else {
        return unauthorized("Unauthorized", TEXT_PLAIN);
    }
}

bool consttimeStrcmp(const char *const a, const char *const b) {
    size_t i = 0;
    unsigned char result = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        result |= a[i] ^ b[i];
        i++;
    }

    result |= a[i] ^ b[i];  

    return result == 0;
}

bool checkBasicCredentials(BasicAuthenticator *auth, const char *const base64) {
    if (!base64) return false;
    if (!auth->credentials) return false;
    
    for (int i = 0; i < auth->credentialsCount; i++) {
        
        if (consttimeStrcmp(auth->credentials[i], base64)) {
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