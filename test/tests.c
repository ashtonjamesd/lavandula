#include <stdio.h>
#include <stdlib.h>
#include "../src/include/lavandula_test.h"

void run_lexer_tests();
void run_parser_tests();
void run_http_tests();
void run_utils_tests();
void run_auth_tests();

int main() {
    printf("=== Lavandula Test Suite ===\n\n");
    
    testsRan = 0;
    testsFailed = 0;
    assertions = 0;
    assertionsFailed = 0;
    
    run_lexer_tests();
    printf("\n");
    
    run_parser_tests();
    printf("\n");
    
    run_http_tests();
    printf("\n");
    
    run_utils_tests();
    printf("\n");
    
    run_auth_tests();
    printf("\n");
    
    printf("=== Final Test Results ===\n");
    testResults();
    
    if (testsFailed > 0 || assertionsFailed > 0) {
        printf("❌ Some tests failed!\n");
        return 1;
    } else {
        printf("✅ All tests passed!\n");
        return 0;
    }
}