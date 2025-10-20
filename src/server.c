#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "include/server.h"
#include "include/http.h"
#include "include/middleware.h"
#include "include/request_context.h"
#include "include/sql.h"
#include "include/app.h"

#define BUFFER_SIZE 4096

HttpResponse defaultNotFoundController(RequestContext context) {
    (void)context;
    return (HttpResponse) {
        .content = "Not Found",
        .status = HTTP_NOT_FOUND
    };
}

HttpResponse defaultMethodNotAllowedController(RequestContext context) {
    (void)context;
    return (HttpResponse) {
        .content = "Method Not Allowed",
        .status = HTTP_METHOD_NOT_ALLOWED
    };
}

Server initServer(int port) {
    Server server;
    server.port = port;
    server.fileDescriptor = -1;

    server.router = initRouter();

    return server;
}

void freeServer(Server *server) {
    if (!server) return;

    if (server->fileDescriptor >= 0) {
        close(server->fileDescriptor);
        server->fileDescriptor = -1;
    }

    freeRouter(&server->router);
}

void runServer(App *app) {
    if (!app) return;

    app->server.fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (app->server.fileDescriptor < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(app->server.fileDescriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(app->server.port);

    if (bind(app->server.fileDescriptor, (struct sockaddr *)&address, sizeof(address)) < 0) {
        if (errno == EADDRINUSE) {
            fprintf(stderr, "Port %d is already in use. Please choose a different port.\n", app->server.port);
        } else {
            fprintf(stderr, "Failed to bind to port %d: %s\n", app->server.port, strerror(errno));
        }

        exit(EXIT_FAILURE);
    }

    if (listen(app->server.fileDescriptor, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Lavandula Server is running! -> http://127.0.0.1:%d\n", app->server.port);

    while (1) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);

        int clientSocket = accept(app->server.fileDescriptor, (struct sockaddr *)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            perror("accept failed");
            continue;
        }

        char buffer[BUFFER_SIZE] = {0}; // oops
        ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
        if (bytesRead < 0) {
            perror("read failed");
            close(clientSocket);
            continue;
        }

        HttpParser parser = parseRequest(buffer);
        HttpRequest request = parser.request;

        char *pathOnly = strdup(request.resource);
        char *queryStart = strchr(pathOnly, '?');
        if (queryStart) {
            *queryStart = '\0';
        }

        Route *route = findRoute(app->server.router, request.method, pathOnly);
        bool routeOfAnyMethodExists = pathExists(app->server.router, pathOnly);

        if (!route && !routeOfAnyMethodExists) {
            Route *notFoundRoute = findRoute(app->server.router, request.method, "/404");

            if (notFoundRoute) {
                route = notFoundRoute;
            }
        }

        free(pathOnly);


        RequestContext context = requestContext(app, request);

        context.hasBody = parser.isValid && request.bodyLength > 0;
        context.body = context.hasBody ? jsonParse(request.body) : NULL;

        HttpResponse response;
        if (route) {
            app->middleware.current = 0;

            MiddlewareHandler combinedMiddleware = combineMiddleware(&app->middleware, route->middleware);
            response = next(context, &combinedMiddleware);
            
            free(combinedMiddleware.handlers);
        } else {
            app->middleware.current = 0;
            app->middleware.finalHandler = routeOfAnyMethodExists ? defaultMethodNotAllowedController : defaultNotFoundController;
            response = next(context, &app->middleware);
        }

        freeJsonBuilder(context.body);

        if (!response.content) {
            response.content = strdup("");
        }

        int contentLength = strlen(response.content);

        const char *statusText = httpStatusCodeToStr(response.status);

        char header[512];
        snprintf(header, sizeof(header),
                "HTTP/1.1 %d %s\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %d\r\n"
                "Connection: close\r\n"
                "\r\n",
                response.status, statusText, response.contentType, contentLength
        );

        if (write(clientSocket, header, strlen(header)) == -1) {
            perror("write header failed");
            exit(EXIT_FAILURE);
        }
        if (write(clientSocket, response.content, contentLength) == -1) {
            perror("write content failed");
            exit(EXIT_FAILURE);
        }

        close(clientSocket);
    }
}
