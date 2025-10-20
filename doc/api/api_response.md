# Error Handling

You have the freedom to handle errors however you like. However, Lavandula provides a interface for returning JSON errors from your controllers.

An API response can be created with two methods.

```
HttpResponse apiSuccess();
HttpResponse apiFailure(char *message);
```

These methods simply create a JSON object with two fields: `success` and `message`. The success field is set to true or false depending on which method you call. The message is only required for returning errors.

```
return apiFailure("missing body in request");
```

This is a method commonly used in middleware. The point of it is to refrain from returning plain text error messages as JSON is easier to work with and standardise.

Here is an example of this used in some middleware.

```
middleware(validateJsonBody, ctx, m) {
    if (!ctx.hasBody) {
        return apiFailure("Error: no JSON body provided.");
    }

    return next(ctx, m);
}
```

If you want to add additional fields onto the API response, you can call the following method instead.s

```
JsonBuilder *apiResponse(char *message, bool success);
```

This method will return the JSON builder, allowing you to put additional fields onto the JSON object. This may be metadata or additional data you wish to return to the caller.