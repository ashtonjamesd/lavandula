#ifndef context_h
#define context_h

#include "sql.h"
#include "http.h"
#include "json.h"
#include "arena.h"

typedef struct App App; 

typedef struct {
    App         *app;

    DbContext   *db;
    HttpRequest  request;

    JsonBuilder *body;
    bool         hasBody;
    
    // Per-request arena allocator for efficient memory management
    // Use arenaAlloc/arenaStrdup for request-scoped allocations
    // Memory is automatically freed when request completes
    Arena       *arena;
} RequestContext;

RequestContext requestContext(App *app, HttpRequest request);

#endif