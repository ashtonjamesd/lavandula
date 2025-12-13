#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <termios.h>
#include <fcntl.h>

#include "include/server.h"
#include "include/http.h"
#include "include/middleware.h"
#include "include/request_context.h"
#include "include/sql.h"
#include "include/app.h"
#include "include/arena.h"
#include "include/lavu_error.h"

typedef enum {
    STATE_RUNNING,
    STATE_RESTARTING,
    STATE_SHUTDOWN
} ServerState;

volatile ServerState serverState = STATE_RUNNING;

#define BUFFER_SIZE 4096

void set_nonblocking_input() {
    struct termios ttystate;

    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

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

void* key_listener(void* arg) {
    (void)arg;

    while (serverState == STATE_RUNNING) {
        int ch = getchar();
        if (ch != EOF) {
            if (ch == 'r') {
                printf("restarting server...\n");
                serverState = STATE_RESTARTING;
                return NULL;
            } else if (ch == 'q') {
                printf("shutting down server...\n");
                serverState = STATE_SHUTDOWN;
                return NULL;
            }
        }
        usleep(10000);
    }
    return NULL;
}


void runServer(App *app) {
    if (!app) return;

    pthread_t thread_id;
    set_nonblocking_input();

    if (pthread_create(&thread_id, NULL, key_listener, NULL)) {
        perror("Failed to create thread");
        return;
    }

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

    printf("\n");
    printf("┌───────────────────────────────────────────────┐\n");
    printf("│         🌿 Lavandula Server is RUNNING        │\n");
    printf("├───────────────────────────────────────────────┤\n");
    printf("│ Listening on: http://127.0.0.1:%-12d   │\n", app->server.port);
    printf("│                                               │\n");
    printf("│ Controls:                                     │\n");
    printf("│   • Press 'r' to reload the server            │\n");
    printf("│   • Press 'q' to shut down                    │\n");
    printf("└───────────────────────────────────────────────┘\n\n");


    int flags = fcntl(app->server.fileDescriptor, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl get flags failed");
        exit(EXIT_FAILURE);
    }
    if (fcntl(app->server.fileDescriptor, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl set non-blocking failed");
        exit(EXIT_FAILURE);
    }

    while (serverState == STATE_RUNNING) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);

        int clientSocket = accept(app->server.fileDescriptor, (struct sockaddr *)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                usleep(10000);
                continue;
            } else {
                perror("accept failed");
                continue;
            }
        }

        // TODO: implement dynamic buffer resizing and reading request in chunks
        char buffer[BUFFER_SIZE] = {0};
        ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
        if (bytesRead < 0) {
            perror("read failed");
            close(clientSocket);
            continue;
        }

        // Create per-request arena for efficient memory management
        Arena *requestArena = arenaCreate();
        if (!requestArena) {
            fprintf(stderr, "Warning: failed to create request arena, falling back to malloc\n");
        }

        HttpParser parser = parseRequest(buffer);
        HttpRequest request = parser.request;

        // Use arena for path string if available, otherwise fall back to strdup
        char *pathOnly = requestArena 
            ? arenaStrdup(requestArena, request.resource)
            : strdup(request.resource);
        
        if (!pathOnly) {
            fprintf(stderr, "Error: out of memory for path\n");
            freeParser(&parser);
            arenaDestroy(requestArena);
            close(clientSocket);
            continue;
        }
        
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

        // Only free pathOnly if we used strdup
        if (!requestArena) {
            free(pathOnly);
        }

        RequestContext context = requestContext(app, request);
        context.arena = requestArena;  // Attach arena to context for middleware/handlers

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

        // Handle NULL response content
        const char *responseContent = response.content ? response.content : "";
        bool needsFreeContent = false;
        
        if (!response.content) {
            // Use arena if available for empty string, otherwise use static
            responseContent = "";
        }

        int contentLength = strlen(responseContent);

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
        } else if (write(clientSocket, responseContent, contentLength) == -1) {
            perror("write content failed");
        }

        if (needsFreeContent && response.content) {
            free(response.content);
        }

        freeParser(&parser);
        
        // Destroy per-request arena - frees all arena allocations at once
        arenaDestroy(requestArena);
        
        close(clientSocket);
    }

    freeServer(&app->server);
    pthread_join(thread_id, NULL);

    if (serverState == STATE_RESTARTING) {
        int result = system("make -s");
        if (result != 0) {
            fprintf(stderr, "Build failed.\n");
            exit(1);
        }

        char *args[] = {"./build/a", NULL};
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    } else if (serverState == STATE_SHUTDOWN) {
        exit(0);
    }
}