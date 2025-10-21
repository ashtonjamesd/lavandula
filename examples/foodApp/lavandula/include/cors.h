#ifndef cors_h
#define cors_h

#include "http.h"

typedef struct {
    char     **allowOrigin;
    int        allowOriginCount;

    HttpMethod allowMethods[6];
    int        methodCount;

    char     **allowHeaders;     
    int        headerCount;
} CorsConfig;

CorsConfig corsPolicy(void);
void freeCorsPolicy(CorsConfig config);

void allowOrigin(CorsConfig *const config, const char *const origin);
void allowMethod(CorsConfig *const config, HttpMethod method);

void allowAnyOrigin(CorsConfig *const config);
void allowAnyMethod(CorsConfig *const config);

void allowHeader(CorsConfig *const config, const char *const header);
void allowAnyHeader(CorsConfig *const config);

CorsConfig corsAllowAll(void);

#endif