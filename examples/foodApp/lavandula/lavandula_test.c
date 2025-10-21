#include <stdio.h>
#include "include/lavandula_test.h"

int testsRan = 0;
int testsFailed = 0;
int assertions = 0;
int assertionsFailed = 0;
const char* currentTestName = NULL;
static int currentTestFailed = 0;

void testFailed() {
    assertionsFailed++;

    if (!currentTestFailed) {
        testsFailed++;
        currentTestFailed = 1;
    }
}

void testResults() {
    printf("\n\nTest Results:\n");
    printf("  tests ran:         %d\n", testsRan);
    printf("  tests failed:      %d\n", testsFailed);
    printf("  assertions:        %d\n", assertions);
    printf("  assertions failed: %d\n", assertionsFailed);
    printf("\n");
}