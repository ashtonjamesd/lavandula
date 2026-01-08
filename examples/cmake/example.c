#include "lavandula.h" 

// define a route for your app
appRoute(home, ctx) {
    (void)ctx;
    return ok("Hello, World", TEXT_PLAIN);
}

int main() {
    // initialise your app
    App app = createApp();

    // register a route in your app
    get(&app, "/home", home);

    // run the app
    runApp(&app);
}
