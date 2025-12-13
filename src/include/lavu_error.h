#ifndef lavu_error_h
#define lavu_error_h

#include <stdbool.h>

// Common error codes used throughout Lavandula
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

// Convert error code to human-readable string
const char *lavuErrorStr(LavuError err);

// Check if result is an error
static inline bool lavuIsError(LavuError err) {
    return err < LAVU_OK;
}

// Macro for early return on error
#define LAVU_TRY(expr) \
    do { \
        LavuError _err = (expr); \
        if (lavuIsError(_err)) { \
            return _err; \
        } \
    } while (0)

// Macro for goto cleanup on error
#define LAVU_TRY_CLEANUP(expr, label) \
    do { \
        LavuError _err = (expr); \
        if (lavuIsError(_err)) { \
            goto label; \
        } \
    } while (0)

// Safe allocation macros that return error instead of crashing
#define LAVU_ALLOC(ptr, size) \
    do { \
        (ptr) = malloc(size); \
        if (!(ptr)) { \
            return LAVU_ERR_NOMEM; \
        } \
    } while (0)

#define LAVU_CALLOC(ptr, count, size) \
    do { \
        (ptr) = calloc((count), (size)); \
        if (!(ptr)) { \
            return LAVU_ERR_NOMEM; \
        } \
    } while (0)

#define LAVU_STRDUP(dest, src) \
    do { \
        if ((src)) { \
            (dest) = strdup(src); \
            if (!(dest)) { \
                return LAVU_ERR_NOMEM; \
            } \
        } else { \
            (dest) = NULL; \
        } \
    } while (0)

// Arena-based allocation macros (preferred for request handling)
#define LAVU_ARENA_ALLOC(ptr, arena, size) \
    do { \
        (ptr) = arenaAlloc((arena), (size)); \
        if (!(ptr)) { \
            return LAVU_ERR_NOMEM; \
        } \
    } while (0)

#define LAVU_ARENA_STRDUP(dest, arena, src) \
    do { \
        if ((src)) { \
            (dest) = arenaStrdup((arena), (src)); \
            if (!(dest)) { \
                return LAVU_ERR_NOMEM; \
            } \
        } else { \
            (dest) = NULL; \
        } \
    } while (0)

#endif
