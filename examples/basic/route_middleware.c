#include "include/lavandula.h"

HttpResponse globalMiddleware(RequestContext ctx, MiddlewareHandler *middleware) {
    printf("Global middleware: Before request processing\n");
    
    return next(ctx, middleware);
}

appRoute(home, ctx) {
    return ok("Hello, World", TEXT_PLAIN);
}

int main() {
    AppBuilder builder = createBuilder();

    // sets up global middleware
    useMiddleware(&builder, globalMiddleware);
    
    usePort(&builder, 3001);
    App app = build(builder);

    // sets up middleware specific to this route
    Route homeRoute = root(&app, home);
    useRouteMiddleware(&homeRoute, consoleLogger);

    runApp(&app);
}