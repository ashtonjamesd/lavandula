#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/include/lavandula_test.h"
#include "../src/include/auth.h"
#include "../src/include/base64.h"

void test_basic_auth_credentials() {
    BasicAuthenticator auth = initBasicAuth();
    addBasicCredentials(&auth, "admin", "secret123");
    addBasicCredentials(&auth, "user", "pass");

    char *authStr1 = base64Encode("admin:secret123");
    char *authStr2 = base64Encode("user:pass");
    char *authStr3 = base64Encode("admin:wrong");
    char *authStr4 = base64Encode("nonexistent:user");

    expect(checkBasicCredentials(&auth, authStr1), toBe(1));
    expect(checkBasicCredentials(&auth, authStr2), toBe(1));
    expect(checkBasicCredentials(&auth, authStr3), toBe(0));
    expect(checkBasicCredentials(&auth, authStr4), toBe(0));

    free(authStr1); free(authStr2); free(authStr3); free(authStr4);
    freeBasicAuth(auth);
}

void test_basic_auth_empty_credentials() {
    BasicAuthenticator auth = initBasicAuth();
    char *authStr = base64Encode("admin:secret");
    expect(checkBasicCredentials(&auth, authStr), toBe(0));
    free(authStr);
    freeBasicAuth(auth);
}

void run_auth_tests() {
    runTest(test_basic_auth_credentials);
    runTest(test_basic_auth_empty_credentials);
}
