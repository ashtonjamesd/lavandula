
#include "../include/lavandula.h"

typedef struct {
    int id;
    char title[100];
    bool completed;
} Todo;

Todo rowToTodo(DbRow row) {
    Todo todo;

    todo.id = atoi(row.colValues[0]);
    strcpy(todo.title, row.colValues[1]);
    todo.completed = atoi(row.colValues[2]);

    return todo;
}

Json todoToJson(Todo todo) {
    JsonBuilder *builder = jsonBuilder();

    jsonPutInteger(builder, "id", todo.id);
    jsonPutString(builder, "title", todo.title);
    jsonPutBool(builder, "completed", todo.completed);

    return jsonObject(builder);
}

appRoute(getTodos, ctx) {
    DbResult *result = dbQueryRows(ctx.db, "select * from todos;", NULL, 0);
    if (!result) { 
        return internalServerError("Database query failed", TEXT_PLAIN); 
    }

    JsonBuilder *root = jsonBuilder();
    JsonArray array = jsonArray();
    jsonPutArray(root, "todos", &array);
    
    for (int i = 0; i < result->rowCount; i++) {
        jsonArrayAppend(&array, todoToJson(rowToTodo(result->rows[i])));
    }

    char *json = jsonStringify(root);
    freeJsonBuilder(root);

    return ok(json, APPLICATION_JSON);
}

appRoute(createTodo, ctx) {
    JsonBuilder *builder = jsonParse(ctx.request.body);

    if (!jsonHasKey(builder, "title")) { 
        return internalServerError("Missing 'title' in request body", TEXT_PLAIN); 
    }

    char *name = jsonGetString(builder, "title");
    bool completed = jsonGetBool(builder, "completed");

    DbParam *params = DB_PARAMS(
        PARAM_TEXT(name),
        PARAM_BOOL(completed)
    );

    bool result = dbExec(ctx.db, "insert into todos (title, completed) values (?, ?);", params, 2);

    if (!result) { 
        return internalServerError("Failed to create todo", TEXT_PLAIN); 
    }

    return ok("ok", TEXT_PLAIN);
}

appRoute(updateTodo, ctx) {
    JsonBuilder *builder = jsonParse(ctx.request.body);

    if (!jsonHasKey(builder, "id")) { 
        return internalServerError("Missing 'id' in request body", TEXT_PLAIN); 
    }

    int id = jsonGetInteger(builder, "id");
    char *title = jsonGetString(builder, "title");
    bool completed = jsonGetBool(builder, "completed");

    DbParam *params = DB_PARAMS(
        PARAM_TEXT(title),
        PARAM_BOOL(completed),
        PARAM_INT(id)
    );

    bool result = dbExec(ctx.db, "update todos set title = ?, completed = ? where id = ?;", params, 3);

    if (!result) { 
        return internalServerError("Failed to update todo", TEXT_PLAIN); 
    }

    return ok("ok", TEXT_PLAIN);
}

appRoute(deleteTodo, ctx) {
    JsonBuilder *builder = jsonParse(ctx.request.body);

    if (!jsonHasKey(builder, "id")) { 
        return internalServerError("Missing 'id' in request body", TEXT_PLAIN); 
    }

    int id = jsonGetInteger(builder, "id");

    DbParam *params = DB_PARAMS(
        PARAM_INT(id)
    );

    bool result = dbExec(ctx.db, "delete from todos where id = ?;", params, 1);

    if (!result) { 
        return internalServerError("Failed to delete todo", TEXT_PLAIN); 
    }

    return ok("ok", TEXT_PLAIN);
}

appRoute(getTodo, ctx) {
    JsonBuilder *builder = jsonParse(ctx.request.body);

    if (!jsonHasKey(builder, "id")) { 
        return internalServerError("Missing 'id' in request body", TEXT_PLAIN); 
    }

    int id = jsonGetInteger(builder, "id");

    DbParam *params = DB_PARAMS(
        PARAM_INT(id)
    );

    DbResult *result = dbQueryRows(ctx.db, "select * from todos where id = ?;", params, 1);
    if (!(result)) { 
        return internalServerError("Database query failed", TEXT_PLAIN); 
    }

    if (result->rowCount == 0) {
        return internalServerError("Todo not found", TEXT_PLAIN);
    }

    JsonBuilder *root = jsonBuilder();

    Json todo = todoToJson(rowToTodo(result->rows[0]));
    jsonPutJson(root, "todo", todo);

    char *json = jsonStringify(root);
    return ok(json, APPLICATION_JSON);
}

int main(int argc, char *argv[]) {
    AppBuilder builder = createBuilder();
    useSqlLite3(&builder, "init.db");
    
    App app = build(builder);

    get(&app, "/getTodos", getTodos);
    get(&app, "/getTodo", getTodo);
    post(&app, "/createTodo", createTodo);
    post(&app, "/updateTodo", updateTodo);
    post(&app, "/deleteTodo", deleteTodo);

    runApp(&app);

    return 0;
}