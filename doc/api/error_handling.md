# Error Handling

Lavandula provides a structured error handling system that allows functions to return error codes instead of crashing the application. This enables graceful degradation and better error recovery.

## Error Codes

All error codes are defined in the `LavuError` enum:

```c
#include "include/lavu_error.h"

typedef enum {
    LAVU_OK = 0,                    // Success
    LAVU_ERR_NOMEM = -1,            // Memory allocation failed
    LAVU_ERR_NULL_PTR = -2,         // Null pointer passed
    LAVU_ERR_INVALID_ARG = -3,      // Invalid argument
    LAVU_ERR_BUFFER_TOO_SMALL = -4, // Buffer too small
    LAVU_ERR_PARSE_FAILED = -5,     // Parsing failed
    LAVU_ERR_NOT_FOUND = -6,        // Item not found
    LAVU_ERR_IO = -7,               // I/O error
    LAVU_ERR_VALIDATION = -8,       // Validation failed
} LavuError;
```

## API Reference

### Error Checking

```c
// Convert error code to human-readable string
const char *msg = lavuErrorStr(LAVU_ERR_NOMEM);
// Returns: "Memory allocation failed"

// Check if a result is an error
if (lavuIsError(result)) {
    // Handle error
}
```

### Error Propagation Macros

```c
// Early return on error
LavuError myFunction() {
    LAVU_TRY(someOperation());  // Returns if error
    LAVU_TRY(anotherOperation());
    return LAVU_OK;
}

// Goto cleanup on error
LavuError myFunction() {
    Resource *res = NULL;
    
    LAVU_TRY_CLEANUP(allocateResource(&res), cleanup);
    LAVU_TRY_CLEANUP(useResource(res), cleanup);
    
    freeResource(res);
    return LAVU_OK;

cleanup:
    freeResource(res);
    return LAVU_ERR_NOMEM;
}
```

### Safe Allocation Macros

```c
// Safe malloc (returns LAVU_ERR_NOMEM on failure)
LavuError myFunction() {
    char *buffer;
    LAVU_ALLOC(buffer, 1024);  // Returns error if malloc fails
    
    // Use buffer...
    
    free(buffer);
    return LAVU_OK;
}

// Safe calloc
int *array;
LAVU_CALLOC(array, 100, sizeof(int));

// Safe strdup
char *copy;
LAVU_STRDUP(copy, originalString);

// Arena-based allocations
void *ptr;
LAVU_ARENA_ALLOC(ptr, arena, 256);

char *str;
LAVU_ARENA_STRDUP(str, arena, "Hello");
```

## Safe API Variants

Many Lavandula functions have "safe" variants that return error codes:

### Validator

```c
// Legacy API (exits on failure)
void addRule(JsonValidator *v, const char *field, const char *message);
void required(JsonValidator *v, const char *field);

// Safe API (returns error code)
LavuError addRuleSafe(JsonValidator *v, const char *field, const char *message);
LavuError requiredSafe(JsonValidator *v, const char *field);
```

### Example: Using Safe Validator API

```c
HttpResponse createUser(RequestContext ctx) {
    JsonValidator v = createValidator();
    
    LavuError err = requiredSafe(&v, "username");
    if (lavuIsError(err)) {
        freeValidator(&v);
        return internalServerError("Validator error", TEXT_PLAIN);
    }
    
    err = requiredSafe(&v, "email");
    if (lavuIsError(err)) {
        freeValidator(&v);
        return internalServerError("Validator error", TEXT_PLAIN);
    }
    
    if (!validate(&v, ctx.body)) {
        char *errorJson = jsonError(v.error);
        freeValidator(&v);  // Must free on validation failure
        return badRequest(errorJson, APPLICATION_JSON);
    }
    
    // Validation passed - validator auto-freed
    // Process request...
    
    return created("{\"id\": 1}", APPLICATION_JSON);
}
```

## Best Practices

1. **Prefer safe variants** for library code that should not crash.

2. **Use legacy variants** for application code where crashing on OOM is acceptable.

3. **Always check returns** from functions that return `LavuError`.

4. **Clean up on error paths**: Use `LAVU_TRY_CLEANUP` for resource management.

5. **Log errors** before returning them for debugging:

```c
if (lavuIsError(err)) {
    fprintf(stderr, "Operation failed: %s\n", lavuErrorStr(err));
    return err;
}
```

## Error Handling Philosophy

Lavandula takes a practical approach to error handling:

- **Critical server errors** (can't bind socket, etc.) still exit
- **Per-request errors** are handled gracefully with error responses
- **Memory errors** during request processing return 500 errors
- **Validation errors** return 400 errors with helpful messages

This ensures the server stays running even when individual requests fail.
