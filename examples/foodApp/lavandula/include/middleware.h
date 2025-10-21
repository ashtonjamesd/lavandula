
// the prefix 'lavandula_' is used to avoid name clashes with header guards
// the user likely will have a file named middleware.h in their project
#ifndef lavandula_middleware_h
#define lavandula_middleware_h

#include "http.h"
#include "router.h"

typedef struct MiddlewareHandler MiddlewareHandler;

// A middleware function takes in a HttpRequest and a pointer to the next middleware handler.
// Returns NULL to continue to next middleware, or an HttpResponse to short-circuit the pipeline.
typedef HttpResponse (* MiddlewareFunc)(RequestContext, MiddlewareHandler *);

struct MiddlewareHandler {
    MiddlewareFunc *handlers;
    int count;
    int capacity;
    int current;
    Controller finalHandler;
};

HttpResponse next(RequestContext context, MiddlewareHandler *middleware);
void useLocalMiddleware(Route *route, MiddlewareFunc handler);
MiddlewareHandler combineMiddleware(MiddlewareHandler *globalMiddleware, MiddlewareHandler *routeMiddleware);

#endif