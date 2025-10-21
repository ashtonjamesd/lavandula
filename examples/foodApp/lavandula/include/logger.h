#ifndef logger_h
#define logger_h

#include "http.h"
#include "router.h"
#include "middleware.h"

#define middleware(name, ctx, m) HttpResponse name(RequestContext ctx, MiddlewareHandler *m)

middleware(consoleLogger, ctx, m);

#endif