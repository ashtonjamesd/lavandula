#include "include/request_context.h"
#include "include/app.h"

RequestContext requestContext(App *app, HttpRequest request) {
    return (RequestContext) {
        .app = app,
        .request = request,
        .db = app->dbContext,
    };
}