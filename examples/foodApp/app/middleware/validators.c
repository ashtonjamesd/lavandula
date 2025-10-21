#include "../../lavandula/include/lavandula.h"
#include "middleware.h"
#include "api_response.h"
#include "validator.h"

middleware(registerUserValidator, ctx, m) {
    JsonValidator v = createValidator();

    required(&v, "username");
    required(&v, "name");
    required(&v, "password");

    if (!validate(&v, ctx.body)) {
        return apiFailure(v.error);
    }

    return next(ctx, m);
}

middleware(loginUserValidator, ctx, m) {
    JsonValidator v = createValidator();

    required(&v, "username");
    required(&v, "password");

    if (!validate(&v, ctx.body)) {
        return apiFailure(v.error);
    }

    return next(ctx, m);
}