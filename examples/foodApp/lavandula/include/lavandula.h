#ifndef lavandula_h
#define lavandula_h

// common headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cli.h"
#include "server.h"
#include "dotenv.h"
#include "middleware.h"
#include "logger.h"
#include "lavandula_test.h"
#include "json.h"
#include "cors.h"
#include "environment.h"
#include "sql.h"
#include "lavender.h"
#include "utils.h"
#include "auth.h"
#include "api_response.h"

#include "version.h"
#include "app.h"

// some useful macros

#define appRoute(name, ctx) HttpResponse name(RequestContext ctx)

#define appRouteStatic(name, path) appRoute(name, ctx) {  \
    if (!ctx.app) exit(1); \
    char *content = readFile(path); \
    return ok(content ? content : "Not Found", TEXT_HTML); \
} \

typedef struct {
    App app;
} AppBuilder;

AppBuilder createBuilder();
App createApp();

// sets the port for the application (default is 3000)
void usePort(AppBuilder *builder, int port);

// adds a middleware function to the application pipeline for all requests
void useGlobalMiddleware(AppBuilder *builder, MiddlewareFunc);

// enables verbose logging for requests and responses
void useVerboseLogging(AppBuilder *builder);

// loads environment variables from a .env file
void useDotenv(char *path);

// adds CORS policy to the application
void useCorsPolicy(AppBuilder *builder, CorsConfig);

// adds a default permissive CORS policy to the application
void useDefaultCorsPolicy(AppBuilder *builder);

// enables HTTPS redirection for all incoming HTTP requests
void useHttpsRedirect(AppBuilder *builder);

// sets the application environment (development, production, testing)
void useEnvironment(AppBuilder *builder, char *env);

// integrates SQLite3 database with the application
void useSqlLite3(AppBuilder *builder, char *dbPath);

// integrates Lavender ORM with the application
void useLavender(AppBuilder *builder);

//
// void useIpRateLimiter(AppBuilder *builder);

// puts basic authentication on all routes
void useBasicAuth(AppBuilder *builder);

bool isDevelopment(AppBuilder *builder);
bool isProduction(AppBuilder *builder);
bool isTesting(AppBuilder *builder);

App build(AppBuilder builder);

void runApp(App *app);
void cleanupApp(App *app);

Route get(App *app, char *path, Controller controller);
Route post(App *app, char *path, Controller controller);
Route put(App *app, char *path, Controller controller);
Route delete(App *app, char *path, Controller controller);
Route patch(App *app, char *path, Controller controller);
Route options(App *app, char *path, Controller controller);

// copy Rails essentially lmao
void resource(App *app, char *resource, Controller (*controllerFactory)());

// defines a special route for handling 404s
Route routeNotFound(App *app,  Controller controller);

// defines a special route for handling the root path
Route root(App *app, Controller controller);


#endif