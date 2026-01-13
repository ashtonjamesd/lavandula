
# Routing

The Lavandula API provides six routing functions, each corresponding to a HTTP method. These functions register the controllers you have created to your web application.

The routing API has since been updated with the following macros, removing redundant parameters.

```c
#define GET(app, controller) get(app, "/" #controller, controller)
#define POST(app, controller) post(app, "/" #controller, controller)

// ..
```

The following functions are still available for use.

```c
Route get(App *app, char *path, Controller controller);
Route post(App *app, char *path, Controller controller);
Route put(App *app, char *path, Controller controller);
Route delete(App *app, char *path, Controller controller);
Route patch(App *app, char *path, Controller controller);
Route options(App *app, char *path, Controller controller);
```

There are two additional special routes you can define in your web application. The first is the root url, which can be set with:

```c
root(&app, home);
```

The second is the route for when a resource requested cannot be found.

```c
HttpResponse notFound(HttpRequest _) {
    return response("Not Found", HTTP_NOT_FOUND);
}

routeNotFound(&app.server.router, notFound);
```
