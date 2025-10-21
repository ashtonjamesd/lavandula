#ifndef context_h
#define context_h

#include "sql.h"
#include "http.h"
#include "json.h"

typedef struct App App; 

typedef struct {
    App         *app;

    DbContext   *db;
    HttpRequest  request;

    JsonBuilder *body;
    bool         hasBody;
} RequestContext;

RequestContext requestContext(App *app, HttpRequest request);

#endif