#include "../src/include/lavandula.h"

#include "../include/lavandula.h"

appRoute(home, ctx) {
    return ok("Hello, World!", TEXT_PLAIN);
}

int main(int argc, char *argv[]) {
    AppBuilder builder = createBuilder();

    // add basic auth into the global middleware pipeline (for all endpoints)
    useBasicAuth(&builder);

    App app = build(builder);

    // add some sample credentials
    addBasicCredentials(&app.auth, "admin", "password");

    get(&app, "/home", home);

    runApp(&app);
}