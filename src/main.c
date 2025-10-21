#include "include/lavandula.h"

appRoute(home, ctx) {
    (void)ctx;
    return apiFailure("An error occurred.");
}

int main() {
    AppBuilder builder = createBuilder();
    App app = build(builder);

    get(&app, "/home", home);

    runApp(&app);

    return 0;
}