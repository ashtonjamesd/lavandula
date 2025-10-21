#ifndef test_h
#define test_h

#include <stdio.h>

extern int testsRan;
extern int testsFailed;
extern int assertions;
extern int assertionsFailed;
extern const char* currentTestName;

typedef void (*TestFunction)();

void runTest(TestFunction);
void testResults();
void testFailed();

#define _assert(cond, x_str, msg) do { \
    assertions++; \
    if (!(cond)) { \
        assertionsFailed++; \
        printf("FAILED: %s at line %d - expected %s %s\n", \
               currentTestName ? currentTestName : "Unknown Test", \
               __LINE__, x_str, msg); \
        testFailed(); \
    } \
} while(0)

#define expect(x, n) _assert((x) == (n), #x, "to be " #n)
#define expectNot(x, n) _assert((x) != (n), #x, "to not be " #n)
#define expectNull(x) _assert((x) == NULL, #x, "to be NULL")
#define expectNotNull(x) _assert((x) != NULL, #x, "to be not NULL")

#define runTest(func) do { \
    currentTestName = #func; \
    func(); \
    testsRan++; \
} while(0)

// just makes it prettier
#define toBe(x) x

#endif