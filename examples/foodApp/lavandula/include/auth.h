#ifndef auth_h
#define auth_h

#include "http.h"
#include "router.h"
#include "middleware.h"

typedef struct {
    char **credentials;
    int    credentialsCount;
    int    credentialsCapacity;
} BasicAuthenticator;

BasicAuthenticator initBasicAuth(void);
HttpResponse basicAuth(RequestContext context, MiddlewareHandler *);
void freeBasicAuth(BasicAuthenticator);

void addBasicCredentials(BasicAuthenticator *auth, const char *const username, const char *const password);
bool checkBasicCredentials(BasicAuthenticator *, const char *const base64);

bool consttimeStrcmp(const char *const a, const char *const b);

#endif