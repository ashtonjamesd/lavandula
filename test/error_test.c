#include <stdio.h>
#include <string.h>
#include "../src/include/lavandula_test.h"
#include "../src/include/lavu_error.h"

void test_lavu_error_str() {
    expect(strcmp(lavuErrorStr(LAVU_OK), "Success"), toBe(0));
    expect(strcmp(lavuErrorStr(LAVU_ERR_NOMEM), "Memory allocation failed"), toBe(0));
    expect(strcmp(lavuErrorStr(LAVU_ERR_NULL_PTR), "Null pointer"), toBe(0));
    expect(strcmp(lavuErrorStr(LAVU_ERR_INVALID_ARG), "Invalid argument"), toBe(0));
    expect(strcmp(lavuErrorStr(LAVU_ERR_BUFFER_TOO_SMALL), "Buffer too small"), toBe(0));
    expect(strcmp(lavuErrorStr(LAVU_ERR_PARSE_FAILED), "Parse failed"), toBe(0));
    expect(strcmp(lavuErrorStr(LAVU_ERR_NOT_FOUND), "Not found"), toBe(0));
    expect(strcmp(lavuErrorStr(LAVU_ERR_IO), "I/O error"), toBe(0));
    expect(strcmp(lavuErrorStr(LAVU_ERR_VALIDATION), "Validation failed"), toBe(0));
}

void test_lavu_is_error() {
    expect(lavuIsError(LAVU_OK), toBe(0));  // false
    expect(lavuIsError(LAVU_ERR_NOMEM), toBe(1));  // true
    expect(lavuIsError(LAVU_ERR_NULL_PTR), toBe(1));  // true
    expect(lavuIsError(LAVU_ERR_VALIDATION), toBe(1));  // true
}

void test_lavu_error_unknown() {
    // Unknown error code should return "Unknown error"
    expect(strcmp(lavuErrorStr((LavuError)-999), "Unknown error"), toBe(0));
}

void runErrorTests() {
    printf("\n--- Error Handling Tests ---\n");
    runTest(test_lavu_error_str);
    runTest(test_lavu_is_error);
    runTest(test_lavu_error_unknown);
}
