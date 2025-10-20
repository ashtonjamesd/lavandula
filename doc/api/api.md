## API Overview

A starter application may look like this:

```c
#include "lavandula.h"

appRoute(home, ctx) {
    return ok("Hello, World!");
}

int main() {
    AppBuilder builder = createBuilder();
    App app = build(builder);

    get(&app, "/home", home);

    runApp(&app);
    cleanupApp(&app);

    return 0;
}
```

Let's break down each part of the program.

This line creates an instance of the application builder. From here, you can call various builder functions that are part of the API to customise your application behaviour. More on this later.

```c
AppBuilder builder = createBuilder();
```

Once the application has been configured, the `build` method is called to produce an `App`. This represents your web application.

```c
App app = build(builder);
```

Before running the app, we need to add some routes so we can serve content from the web application. The line below defines a route for a GET request. The route path is defined as `/home` and a controller function is given to the route. This is the function that runs when a user visits that URL.

The function must return a `HttpResponse` and take in a `HttpRequest`.

```c
get(&app, "/home", home);
```

This line starts the web server. Most of the application runtime is spent inside this function.

```c
runApp(&app);
```

Once the app terminates, you should call this function to clean up leftover resources in memory.

```c
cleanupApp(&app);
```

Try running the above example and expect the following output when you navigate to `/home`.

```
Hello, World!
```

Lavandula also provides a more minimal interface for creating web applications without a builder object. Calling the `createApp` function will return you an app directly without the need for a builder.

```c
#include "lavandula.h"

appRoute(home, ctx) {
    return ok("Hello, World");
}

int main(void) {
    App app = createApp();
    root(&app, home);

    runApp(&app);
}
```

To make life easier when defining routes in your application, you can use the `appRoute` macro. The example above generates the following signature. The variable for accessing the `RequestContext` will be by convention named `ctx` when using the `appRoute` macro.

```c
appRoute(home, ctx) -> HttpResponse home(RequestContext ctx)
```