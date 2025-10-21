#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/middleware.h"

HttpResponse next(RequestContext context, MiddlewareHandler *middleware) {
    while (middleware->current < middleware->count) {
        MiddlewareFunc handler = middleware->handlers[middleware->current++];
        if (handler) {
            HttpResponse response = handler(context, middleware);

            if (response.content != NULL) {
                return response;
            }
        }
    }
    
    if (middleware->finalHandler) {
        return middleware->finalHandler(context);
    }
    
    HttpResponse notFoundResponse = {
        .content = "Not Found",
        .status = HTTP_NOT_FOUND
    };

    return notFoundResponse;
}

void useLocalMiddleware(Route *route, MiddlewareFunc handler) {
    if (route->middleware->count >= route->middleware->capacity) {
        route->middleware->capacity *= 2;
        route->middleware->handlers = realloc(route->middleware->handlers, sizeof(MiddlewareFunc) * route->middleware->capacity);

        if (!route->middleware->handlers) {
            fprintf(stderr, "Fatal: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
    route->middleware->handlers[route->middleware->count++] = handler;
}

MiddlewareHandler combineMiddleware(MiddlewareHandler *globalMiddleware, MiddlewareHandler *routeMiddleware) {
    int totalCount = globalMiddleware->count + routeMiddleware->count;
    
    MiddlewareHandler combined = {
        .handlers = malloc(sizeof(MiddlewareFunc) * totalCount),
        .count = totalCount,
        .capacity = totalCount,
        .current = 0,
        .finalHandler = routeMiddleware->finalHandler
    };

    if (!combined.handlers) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < globalMiddleware->count; i++) {
        combined.handlers[i] = globalMiddleware->handlers[i];
    }
    
    for (int i = 0; i < routeMiddleware->count; i++) {
        combined.handlers[globalMiddleware->count + i] = routeMiddleware->handlers[i];
    }
    
    return combined;
}