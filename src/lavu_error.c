#include "include/lavu_error.h"

const char *lavuErrorStr(LavuError err) {
    switch (err) {
        case LAVU_OK:
            return "Success";
        case LAVU_ERR_NOMEM:
            return "Memory allocation failed";
        case LAVU_ERR_NULL_PTR:
            return "Null pointer";
        case LAVU_ERR_INVALID_ARG:
            return "Invalid argument";
        case LAVU_ERR_BUFFER_TOO_SMALL:
            return "Buffer too small";
        case LAVU_ERR_PARSE_FAILED:
            return "Parse failed";
        case LAVU_ERR_NOT_FOUND:
            return "Not found";
        case LAVU_ERR_IO:
            return "I/O error";
        case LAVU_ERR_VALIDATION:
            return "Validation failed";
        default:
            return "Unknown error";
    }
}
