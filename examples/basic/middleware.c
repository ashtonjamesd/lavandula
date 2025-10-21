#include "lavandula.h"

appRoute(home, ctx) {
    return ok("Hello, World!", TEXT_PLAIN);
}

int main() {
    AppBuilder builder = createBuilder();
    App app = build(builder);

    // Logging middleware example
    useMiddleware(&builder, consoleLogger);

    get(&app, "/home", home);

    runApp(&app);

    return 0;
}