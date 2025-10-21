#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/lavandula.h"

void initAppMiddleware(App *app) {
    app->middleware = (MiddlewareHandler) {
        .handlers = malloc(sizeof(MiddlewareHandler) * 1),
        .count = 0,
        .capacity = 1,
        .current = 0,
    };

    if (!app->middleware.handlers) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }
}

AppBuilder createBuilder() {
    AppBuilder builder = {
        .app = createApp()
    };

    return builder;
}

App createApp() {
    App app = {
        .port = 3000,
    };

    initAppMiddleware(&app);
    app.server = initServer(app.port);

    return app;
}

void usePort(AppBuilder *builder, int port) {
    builder->app.server.port = port;
}

void useGlobalMiddleware(AppBuilder *builder, MiddlewareFunc middleware) {
    if (builder->app.middleware.count >= builder->app.middleware.capacity) {
        builder->app.middleware.capacity *= 2;
        builder->app.middleware.handlers = realloc(builder->app.middleware.handlers, sizeof(MiddlewareFunc) * builder->app.middleware.capacity);

        if (!builder->app.middleware.handlers) {
            fprintf(stderr, "Fatal: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
    builder->app.middleware.handlers[builder->app.middleware.count++] = middleware;
}

void useVerboseLogging(AppBuilder *builder) {
    builder->app.verboseLogging = true;
}

void useCorsPolicy(AppBuilder *builder, CorsConfig corsPolicy) {
    builder->app.corsPolicy = corsPolicy;
}

void useDefaultCorsPolicy(AppBuilder *builder) {
    builder->app.corsPolicy = corsPolicy();

    allowOrigin(&builder->app.corsPolicy, "*");

    allowMethod(&builder->app.corsPolicy, HTTP_GET);
    allowMethod(&builder->app.corsPolicy, HTTP_POST);
    allowMethod(&builder->app.corsPolicy, HTTP_PUT);
    allowMethod(&builder->app.corsPolicy, HTTP_PATCH);
    allowMethod(&builder->app.corsPolicy, HTTP_DELETE);
    allowMethod(&builder->app.corsPolicy, HTTP_OPTIONS);
}

void useHttpsRedirect(AppBuilder *builder) {
    builder->app.useHttpsRedirect = true;
}

void useEnvironment(AppBuilder *builder, char *env) {
    builder->app.environment = env;
}

void useSqlLite3(AppBuilder *builder, char *dbPath) {
    builder->app.dbContext = createSqlLite3DbContext(dbPath);
}

void useLavender(AppBuilder *builder) {
    builder->app.useLavender = true;
}

void useBasicAuth(AppBuilder *builder) {
    useGlobalMiddleware(builder, basicAuth);
}

bool isDevelopment(AppBuilder *builder) {
    char *env = builder->app.environment;
    if (!env) return false;

    if (strcmp(env, "DEVELOPMENT") == 0) {
        return true;
    }

    return false;
}

bool isProduction(AppBuilder *builder) {
    char *env = builder->app.environment;
    if (!env) return false;

    if (strcmp(env, "PRODUCTION") == 0) {
        return true;
    }

    return false;
}

bool isTesting(AppBuilder *builder) {
    char *env = builder->app.environment;
    if (!env) return false;

    if (strcmp(env, "TESTING") == 0) {
        return true;
    }

    return false;
}

void useDotenv(char *path) {
    dotenv(path);
}

App build(AppBuilder builder) {
    return builder.app;
}

void runApp(App *app) {
    runServer(app);
    cleanupApp(app);
}

void cleanupApp(App *app) {
    if (!app) return;
    
    freeServer(&app->server);
    dotenvClean();
    free(app->middleware.handlers);

    if (!app->dbContext) return;
    if (app->dbContext->type == SQLITE) {
        free((char *)app->dbContext->connection);
    }
}

Route get(App *app, char *path, Controller controller) {
    return route(&app->server.router, HTTP_GET, path, controller);
}

Route post(App *app, char *path, Controller controller) {
    return route(&app->server.router, HTTP_POST, path, controller);
}

Route put(App *app, char *path, Controller controller) {
    return route(&app->server.router, HTTP_PUT, path, controller);
}

Route delete(App *app, char *path, Controller controller) {
    return route(&app->server.router, HTTP_DELETE, path, controller);
}

Route patch(App *app, char *path, Controller controller) {
    return route(&app->server.router, HTTP_PATCH, path, controller);
}

Route options(App *app, char *path, Controller controller) {
    return route(&app->server.router, HTTP_OPTIONS, path, controller);
}

void resource(App *app, char *resource, Controller (*controllerFactory)()) {
    if (!(app && resource && controllerFactory)) return;
}

Route routeNotFound(App *app, Controller controller) {
    return route(&app->server.router, HTTP_GET, "/404", controller);
}

Route root(App *app, Controller controller) {
    return route(&app->server.router, HTTP_GET, "/", controller);
}