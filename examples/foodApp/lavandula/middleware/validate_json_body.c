#include "../include/validate_json_body.h"

middleware(validateJsonBody, ctx, m) {
    if (!ctx.hasBody) {
        return apiFailure("Error: no JSON body provided.");
    }

    return next(ctx, m);
}