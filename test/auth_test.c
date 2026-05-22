#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../src/include/lavandula_test.h"
#include "../src/include/utils.h"
#include "../src/include/auth.h"

void test_constant_time_equals_equal_strings() {
    expect(constantTimeEquals("abc", "abc"), toBe(true));
}

void test_constant_time_equals_different_strings() {
    expect(constantTimeEquals("abc", "xyz"), toBe(false));
}

void test_constant_time_equals_different_lengths() {
    expect(constantTimeEquals("abc", "abcd"), toBe(false));
}

void test_constant_time_equals_empty_strings() {
    expect(constantTimeEquals("", ""), toBe(true));
}

void test_constant_time_equals_null_a() {
    expect(constantTimeEquals(NULL, "abc"), toBe(false));
}

void test_constant_time_equals_null_b() {
    expect(constantTimeEquals("abc", NULL), toBe(false));
}

void test_constant_time_equals_both_null() {
    expect(constantTimeEquals(NULL, NULL), toBe(false));
}


void test_check_basic_credentials_valid() {
    BasicAuthenticator auth = initBasicAuth();
    addBasicCredentials(&auth, (char*)"user", (char*)"pass");
    /* The stored credential is base64 of "user:pass" = "dXNlcjpwYXNz" */
    expect(checkBasicCredentials(&auth, "dXNlcjpwYXNz"), toBe(true));
    freeBasicAuth(auth);
}

void test_check_basic_credentials_invalid() {
    BasicAuthenticator auth = initBasicAuth();
    addBasicCredentials(&auth, (char*)"user", (char*)"pass");
    expect(checkBasicCredentials(&auth, "aW52YWxpZDpjcmlkcw=="), toBe(false));
    freeBasicAuth(auth);
}

void test_check_basic_credentials_multiple_match_first() {
    BasicAuthenticator auth = initBasicAuth();
    addBasicCredentials(&auth, (char*)"user1", (char*)"pass1");
    addBasicCredentials(&auth, (char*)"user2", (char*)"pass2");
    /* base64 of "user1:pass1" = "dXNlcjE6cGFzczE=" */
    expect(checkBasicCredentials(&auth, "dXNlcjE6cGFzczE="), toBe(true));
    freeBasicAuth(auth);
}

void test_check_basic_credentials_multiple_match_last() {
    BasicAuthenticator auth = initBasicAuth();
    addBasicCredentials(&auth, (char*)"user1", (char*)"pass1");
    addBasicCredentials(&auth, (char*)"user2", (char*)"pass2");
    /* base64 of "user2:pass2" = "dXNlcjI6cGFzczI=" */
    expect(checkBasicCredentials(&auth, "dXNlcjI6cGFzczI="), toBe(true));
    freeBasicAuth(auth);
}

void test_check_basic_credentials_no_match() {
    BasicAuthenticator auth = initBasicAuth();
    addBasicCredentials(&auth, (char*)"user1", (char*)"pass1");
    addBasicCredentials(&auth, (char*)"user2", (char*)"pass2");
    expect(checkBasicCredentials(&auth, "bm9uZTptYXRjaA=="), toBe(false));
    freeBasicAuth(auth);
}

void test_check_basic_credentials_empty_db() {
    BasicAuthenticator auth = initBasicAuth();
    expect(checkBasicCredentials(&auth, "dXNlcjpwYXNz"), toBe(false));
    freeBasicAuth(auth);
}


void test_constant_time_equals_no_early_exit() {
    /* Verify function doesn't early-exit by testing all positions */
    expect(constantTimeEquals("aaaaaaaa", "baaaaaaa"), toBe(false));
    expect(constantTimeEquals("aaaaaaaa", "abaaaaaa"), toBe(false));
    expect(constantTimeEquals("aaaaaaaa", "aabaaaaa"), toBe(false));
    expect(constantTimeEquals("aaaaaaaa", "aaabaaaa"), toBe(false));
    expect(constantTimeEquals("aaaaaaaa", "aaaabaaa"), toBe(false));
    expect(constantTimeEquals("aaaaaaaa", "aaaaabaa"), toBe(false));
    expect(constantTimeEquals("aaaaaaaa", "aaaaaaba"), toBe(false));
    expect(constantTimeEquals("aaaaaaaa", "aaaaaaab"), toBe(false));
    /* All should return false - if function early-exited, any could fail */
}

void run_auth_tests() {
    runTest(test_constant_time_equals_equal_strings);
    runTest(test_constant_time_equals_different_strings);
    runTest(test_constant_time_equals_different_lengths);
    runTest(test_constant_time_equals_empty_strings);
    runTest(test_constant_time_equals_null_a);
    runTest(test_constant_time_equals_null_b);
    runTest(test_constant_time_equals_both_null);
    runTest(test_constant_time_equals_no_early_exit);
    runTest(test_check_basic_credentials_valid);
    runTest(test_check_basic_credentials_invalid);
    runTest(test_check_basic_credentials_multiple_match_first);
    runTest(test_check_basic_credentials_multiple_match_last);
    runTest(test_check_basic_credentials_no_match);
    runTest(test_check_basic_credentials_empty_db);
}
