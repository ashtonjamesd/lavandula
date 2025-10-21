#ifndef router_h
#define router_h

#include "http.h"
#include "request_context.h"

// forward declaration
typedef struct MiddlewareHandler MiddlewareHandler;

typedef HttpResponse (*Controller)(RequestContext);

typedef struct {
    HttpMethod method;
    char      *path;

    Controller controller;
    MiddlewareHandler *middleware;
} Route;

typedef struct {
    Route *routes;
    int    routeCount;
    int    routeCapacity;
} Router;

Router initRouter();
void freeRouter(Router *router);

Route route(Router *router, HttpMethod method, char *path, Controller controller);

Route *findRoute(Router router, HttpMethod method, char *routePath);
bool pathExists(Router router, char *routePath);

HttpResponse response(char *content, HttpStatusCode, char *contentType);

HttpResponse notImplementedYet();

// 1xx Informational responses
HttpResponse httpContinue(char *content, char *contentType);
HttpResponse switchingProtocols(char *content, char *contentType);
HttpResponse processing(char *content, char *contentType);
HttpResponse earlyHints(char *content, char *contentType);

// 2xx Success responses
HttpResponse ok(char *content, char *contentType);
HttpResponse created(char *content, char *contentType);
HttpResponse accepted(char *content, char *contentType);
HttpResponse nonAuthoritativeInformation(char *content, char *contentType);
HttpResponse noContent(char *content, char *contentType);
HttpResponse resetContent(char *content, char *contentType);
HttpResponse partialContent(char *content, char *contentType);
HttpResponse multiStatus(char *content, char *contentType);
HttpResponse alreadyReported(char *content, char *contentType);
HttpResponse imUsed(char *content, char *contentType);

// 3xx Redirection responses
HttpResponse multipleChoices(char *content, char *contentType);
HttpResponse movedPermanently(char *content, char *contentType);
HttpResponse found(char *content, char *contentType);
HttpResponse seeOther(char *content, char *contentType);
HttpResponse notModified(char *content, char *contentType);
HttpResponse useProxy(char *content, char *contentType);
HttpResponse temporaryRedirect(char *content, char *contentType);
HttpResponse permanentRedirect(char *content, char *contentType);

// 4xx Client error responses
HttpResponse badRequest(char *content, char *contentType);
HttpResponse unauthorized(char *content, char *contentType);
HttpResponse paymentRequired(char *content, char *contentType);
HttpResponse forbidden(char *content, char *contentType);
HttpResponse notFound(char *content, char *contentType);
HttpResponse methodNotAllowed(char *content, char *contentType);
HttpResponse notAcceptable(char *content, char *contentType);
HttpResponse proxyAuthenticationRequired(char *content, char *contentType);
HttpResponse requestTimeout(char *content, char *contentType);
HttpResponse conflict(char *content, char *contentType);
HttpResponse gone(char *content, char *contentType);
HttpResponse lengthRequired(char *content, char *contentType);
HttpResponse preconditionFailed(char *content, char *contentType);
HttpResponse payloadTooLarge(char *content, char *contentType);
HttpResponse uriTooLong(char *content, char *contentType);
HttpResponse unsupportedMediaType(char *content, char *contentType);
HttpResponse rangeNotSatisfiable(char *content, char *contentType);
HttpResponse expectationFailed(char *content, char *contentType);
HttpResponse imATeapot(char *content, char *contentType);
HttpResponse misdirectedRequest(char *content, char *contentType);
HttpResponse unprocessableEntity(char *content, char *contentType);
HttpResponse locked(char *content, char *contentType);
HttpResponse failedDependency(char *content, char *contentType);
HttpResponse tooEarly(char *content, char *contentType);
HttpResponse upgradeRequired(char *content, char *contentType);
HttpResponse preconditionRequired(char *content, char *contentType);
HttpResponse tooManyRequests(char *content, char *contentType);
HttpResponse requestHeaderFieldsTooLarge(char *content, char *contentType);
HttpResponse unavailableForLegalReasons(char *content, char *contentType);

// 5xx Server error responses
HttpResponse internalServerError(char *content, char *contentType);
HttpResponse notImplemented(char *content, char *contentType);
HttpResponse badGateway(char *content, char *contentType);
HttpResponse serviceUnavailable(char *content, char *contentType);
HttpResponse gatewayTimeout(char *content, char *contentType);
HttpResponse httpVersionNotSupported(char *content, char *contentType);
HttpResponse variantAlsoNegotiates(char *content, char *contentType);
HttpResponse insufficientStorage(char *content, char *contentType);
HttpResponse loopDetected(char *content, char *contentType);
HttpResponse notExtended(char *content, char *contentType);
HttpResponse networkAuthenticationRequired(char *content, char *contentType);

#endif