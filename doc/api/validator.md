# JSON Validation

Lavandula provides a JSON validator interface for ensuring the correct data is being sent to your application.

Start by creating a validator instance. Then you can call various methods to add rules to the validator. The example below calls the `required` method to add expected fields in the JSON body.

Then, call `validate` to check the context body against the previously defined rules. It will return `NULL` if there is no error. Lastly, free the validator instance to clean up resources.

```c
middleware(registerUserValidator, ctx, m) {
    JsonValidator v = createValidator();

    required(&v, "username");
    required(&v, "password");

    char *error = validate(&v, ctx.body);
    if (error) {
        return failure(error);
    }
    freeValidator(&v);

    return next(ctx, m);
}
```

The JSON validator provides automatic validation against the presence of a body. If a body is not present and a validator is used then the following response will be returned.

```json
{
    "success": false,
    "message": "Request body is missing or malformed."
}
```