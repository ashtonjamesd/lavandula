#include <stdio.h>

#include "../include/logger.h"

middleware(consoleLogger, ctx, m) {
    printf("Logger: %s: '%s'\n", httpMethodToStr(ctx.request.method), ctx.request.resource);

    return next(ctx, m);
}