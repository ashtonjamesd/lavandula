#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/router.h"
#include "include/server.h"

// 1xx Informational responses
HttpResponse httpContinue(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_CONTINUE,
        .contentType = contentType
    };
}

HttpResponse switchingProtocols(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_SWITCHING_PROTOCOLS,
        .contentType = contentType
    };
}

HttpResponse processing(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_PROCESSING,
        .contentType = contentType
    };
}

HttpResponse earlyHints(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_EARLY_HINTS,
        .contentType = contentType
    };
}

// 2xx Success responses
HttpResponse ok(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_OK,
        .contentType = contentType
    };
}

HttpResponse created(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_CREATED,
        .contentType = contentType
    };
}

HttpResponse accepted(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_ACCEPTED,
        .contentType = contentType
    };
}

HttpResponse nonAuthoritativeInformation(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_NON_AUTHORITATIVE_INFORMATION,
        .contentType = contentType
    };
}

HttpResponse noContent(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_NO_CONTENT,
        .contentType = contentType
    };
}

HttpResponse resetContent(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_RESET_CONTENT,
        .contentType = contentType
    };
}

HttpResponse partialContent(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_PARTIAL_CONTENT,
        .contentType = contentType
    };
}

HttpResponse multiStatus(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_MULTI_STATUS,
        .contentType = contentType
    };
}

HttpResponse alreadyReported(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_ALREADY_REPORTED,
        .contentType = contentType
    };
}

HttpResponse imUsed(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_IM_USED,
        .contentType = contentType
    };
}

// 3xx Redirection responses
HttpResponse multipleChoices(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_MULTIPLE_CHOICES,
        .contentType = contentType
    };
}

HttpResponse movedPermanently(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_MOVED_PERMANENTLY,
        .contentType = contentType
    };
}

HttpResponse found(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_FOUND,
        .contentType = contentType
    };
}

HttpResponse seeOther(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_SEE_OTHER,
        .contentType = contentType
    };
}

HttpResponse notModified(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_NOT_MODIFIED,
        .contentType = contentType
    };
}

HttpResponse useProxy(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_USE_PROXY,
        .contentType = contentType
    };
}

HttpResponse temporaryRedirect(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_TEMPORARY_REDIRECT,
        .contentType = contentType
    };
}

HttpResponse permanentRedirect(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_PERMANENT_REDIRECT,
        .contentType = contentType
    };
}

// 4xx Client error responses
HttpResponse badRequest(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_BAD_REQUEST,
        .contentType = contentType
    };
}

HttpResponse unauthorized(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_UNAUTHORIZED,
        .contentType = contentType
    };
}

HttpResponse paymentRequired(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_PAYMENT_REQUIRED,
        .contentType = contentType
    };
}

HttpResponse forbidden(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_FORBIDDEN,
        .contentType = contentType
    };
}

HttpResponse notFound(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_NOT_FOUND,
        .contentType = contentType
    };
}

HttpResponse methodNotAllowed(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_METHOD_NOT_ALLOWED,
        .contentType = contentType
    };
}

HttpResponse notAcceptable(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_NOT_ACCEPTABLE,
        .contentType = contentType
    };
}

HttpResponse proxyAuthenticationRequired(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_PROXY_AUTHENTICATION_REQUIRED,
        .contentType = contentType
    };
}

HttpResponse requestTimeout(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_REQUEST_TIMEOUT,
        .contentType = contentType
    };
}

HttpResponse conflict(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_CONFLICT,
        .contentType = contentType
    };
}

HttpResponse gone(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_GONE,
        .contentType = contentType
    };
}

HttpResponse lengthRequired(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_LENGTH_REQUIRED,
        .contentType = contentType
    };
}

HttpResponse preconditionFailed(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_PRECONDITION_FAILED,
        .contentType = contentType
    };
}

HttpResponse payloadTooLarge(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_PAYLOAD_TOO_LARGE,
        .contentType = contentType
    };
}

HttpResponse uriTooLong(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_URI_TOO_LONG,
        .contentType = contentType
    };
}

HttpResponse unsupportedMediaType(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_UNSUPPORTED_MEDIA_TYPE,
        .contentType = contentType
    };
}

HttpResponse rangeNotSatisfiable(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_RANGE_NOT_SATISFIABLE,
        .contentType = contentType
    };
}

HttpResponse expectationFailed(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_EXPECTATION_FAILED,
        .contentType = contentType
    };
}

HttpResponse imATeapot(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_IM_A_TEAPOT,
        .contentType = contentType
    };
}

HttpResponse misdirectedRequest(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_MISDIRECTED_REQUEST,
        .contentType = contentType
    };
}

HttpResponse unprocessableEntity(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_UNPROCESSABLE_ENTITY,
        .contentType = contentType
    };
}

HttpResponse locked(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_LOCKED,
        .contentType = contentType
    };
}

HttpResponse failedDependency(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_FAILED_DEPENDENCY,
        .contentType = contentType
    };
}

HttpResponse tooEarly(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_TOO_EARLY,
        .contentType = contentType
    };
}

HttpResponse upgradeRequired(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_UPGRADE_REQUIRED,
        .contentType = contentType
    };
}

HttpResponse preconditionRequired(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_PRECONDITION_REQUIRED,
        .contentType = contentType
    };
}

HttpResponse tooManyRequests(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_TOO_MANY_REQUESTS,
        .contentType = contentType
    };
}

HttpResponse requestHeaderFieldsTooLarge(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE,
        .contentType = contentType
    };
}

HttpResponse unavailableForLegalReasons(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_UNAVAILABLE_FOR_LEGAL_REASONS,
        .contentType = contentType
    };
}

// 5xx Server error responses
HttpResponse internalServerError(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_INTERNAL_SERVER_ERROR,
        .contentType = contentType
    };
}

HttpResponse notImplemented(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_NOT_IMPLEMENTED,
        .contentType = contentType
    };
}

HttpResponse badGateway(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_BAD_GATEWAY,
        .contentType = contentType
    };
}

HttpResponse serviceUnavailable(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_SERVICE_UNAVAILABLE,
        .contentType = contentType
    };
}

HttpResponse gatewayTimeout(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_GATEWAY_TIMEOUT,
        .contentType = contentType
    };
}

HttpResponse httpVersionNotSupported(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_HTTP_VERSION_NOT_SUPPORTED,
        .contentType = contentType
    };
}

HttpResponse variantAlsoNegotiates(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_VARIANT_ALSO_NEGOTIATES,
        .contentType = contentType
    };
}

HttpResponse insufficientStorage(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_INSUFFICIENT_STORAGE,
        .contentType = contentType
    };
}

HttpResponse loopDetected(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_LOOP_DETECTED,
        .contentType = contentType
    };
}

HttpResponse notExtended(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_NOT_EXTENDED,
        .contentType = contentType
    };
}

HttpResponse networkAuthenticationRequired(char *content, char *contentType) {
    return (HttpResponse){
        .content = content,
        .status = HTTP_NETWORK_AUTHENTICATION_REQUIRED,
        .contentType = contentType
    };
}

HttpResponse response(char *content, HttpStatusCode status, char *contentType) {
    return (HttpResponse) {
        .content = content,
        .status = status,
        .contentType = contentType
    };
}

Router initRouter() {
    Router router = {
        .routeCapacity = 1,
        .routeCount = 0,
        .routes = malloc(sizeof(Route))
    };

    if (!router.routes) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return router;
}

void freeRouter(Router *router) {
    if (!router) return;

    for (int i = 0; i < router->routeCount; i++) {
        Route route = router->routes[i];
        free(route.path);
        
        if (route.middleware) {
            free(route.middleware->handlers);
            free(route.middleware);
        }
    }

    free(router->routes);
}

Route route(Router *router, HttpMethod method, char *path, Controller controller) {
    MiddlewareHandler *middleware = malloc(sizeof(MiddlewareHandler));
    *middleware = (MiddlewareHandler){
        .handlers = malloc(sizeof(MiddlewareFunc) * 1),
        .count = 0,
        .capacity = 1,
        .current = 0,
        .finalHandler = controller
    };

    if (!middleware || !middleware->handlers) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    Route route = {
        .method = method,
        .path = strdup(path),
        .controller = controller,
        .middleware = middleware
    };

    if (router->routeCount >= router->routeCapacity) {
        router->routeCapacity *= 2;
        router->routes = realloc(router->routes, sizeof(Route) * router->routeCapacity);

        if (!router->routes) {
            fprintf(stderr, "Fatal: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
    router->routes[router->routeCount++] = route;

    return route;
}

Route *findRoute(Router router, HttpMethod method, char *path) {
    for (int i = 0; i < router.routeCount; i++) {
        Route route = router.routes[i];

        if ((strcmp(route.path, path) == 0) && (route.method == method)) {
            return &router.routes[i];
        }
    }

    return NULL;
}

bool pathExists(Router router, char *path) {
    for (int i = 0; i < router.routeCount; i++) {
        Route route = router.routes[i];

        if (strcmp(route.path, path) == 0) {
            return true;
        }
    }

    return false;
}

HttpResponse notImplementedYet() {
    return notImplemented("This route has not been implemented yet!", TEXT_PLAIN);
}