#include <stdbool.h>

#include "include/lavandula.h"

// Creates a basic API response structure
JsonBuilder *apiResponse(char *message, bool success) {
    JsonBuilder *builder = jsonBuilder();
    jsonPutBool(builder, "success", success);
    jsonPutString(builder, "message", message);

    return builder;
}

// For returning a simple success response
HttpResponse apiSuccess() {
    JsonBuilder *json = apiResponse("Request was successful.", true);
    char *response = jsonStringify(json);
    freeJsonBuilder(json);

    return ok(response, APPLICATION_JSON);
}

// For returning a simple failure response with a message
HttpResponse apiFailure(char *message) {
    JsonBuilder *json = apiResponse(message, false);
    char *response = jsonStringify(json);
    freeJsonBuilder(json);

    return internalServerError(response, APPLICATION_JSON);
}