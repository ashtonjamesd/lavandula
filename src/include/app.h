#ifndef app_h
#define app_h

#include "server.h"
#include "cors.h"
#include "auth.h"

struct App {
    int                port;
    Server             server;

    bool               verboseLogging;
    bool               useHttpsRedirect;
    char              *environment;
    bool               useLavender;     
    MiddlewareHandler  middleware;
    CorsConfig          corsPolicy;
    DbContext         *dbContext;
    BasicAuthenticator auth;
};

#endif