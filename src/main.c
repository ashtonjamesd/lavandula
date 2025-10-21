#include "include/lavandula.h"

appRoute(home, ctx) {
    (void)ctx;
    return apiSuccess();
}

int main() {
    AppBuilder builder = createBuilder();
    App app = build(builder);

    get(&app, "/home", home);

    runApp(&app);

    return 0;
}