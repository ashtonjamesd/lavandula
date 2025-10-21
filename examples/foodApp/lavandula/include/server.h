#ifndef server_h
#define server_h

#include "router.h"
#include "middleware.h"

typedef struct App App;

typedef struct {
    Router router;
    
    int port;
    int fileDescriptor;
} Server;

Server initServer(int port);
void freeServer(Server *server);

void runServer(App *app);

#endif