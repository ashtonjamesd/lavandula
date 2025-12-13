#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../src/include/lavandula_test.h"
#include "../src/include/validator.h"
#include "../src/include/json.h"
#include "../src/include/lavu_error.h"

void test_validator_create() {
    JsonValidator v = createValidator();
    expectNotNull(v.rules);
    expect(v.ruleCount, toBe(0));
    expectNot(v.ruleCapacity, toBe(0));
    freeValidator(&v);
}

void test_validator_add_rule_safe() {
    JsonValidator v = createValidator();
    
    LavuError err = addRuleSafe(&v, "username", "Username is required");
    expect(err, toBe(LAVU_OK));
    expect(v.ruleCount, toBe(1));
    
    err = addRuleSafe(&v, "email", "Email is required");
    expect(err, toBe(LAVU_OK));
    expect(v.ruleCount, toBe(2));
    
    freeValidator(&v);
}

void test_validator_add_rule_safe_null() {
    JsonValidator v = createValidator();
    
    LavuError err = addRuleSafe(&v, NULL, "message");
    expect(err, toBe(LAVU_ERR_NULL_PTR));
    
    err = addRuleSafe(&v, "field", NULL);
    expect(err, toBe(LAVU_ERR_NULL_PTR));
    
    err = addRuleSafe(NULL, "field", "message");
    expect(err, toBe(LAVU_ERR_NULL_PTR));
    
    expect(v.ruleCount, toBe(0));  // No rules should have been added
    
    freeValidator(&v);
}

void test_validator_required_safe() {
    JsonValidator v = createValidator();
    
    LavuError err = requiredSafe(&v, "name");
    expect(err, toBe(LAVU_OK));
    expect(v.ruleCount, toBe(1));
    
    // Check that the message was auto-generated
    expectNotNull(v.rules[0].message);
    
    freeValidator(&v);
}

void test_validator_free_cleans_strings() {
    JsonValidator v = createValidator();
    
    // Add several rules
    addRuleSafe(&v, "field1", "message1");
    addRuleSafe(&v, "field2", "message2");
    addRuleSafe(&v, "field3", "message3");
    
    expect(v.ruleCount, toBe(3));
    
    // Free should clean up all strdup'd strings
    freeValidator(&v);
    
    expectNull(v.rules);
    expect(v.ruleCount, toBe(0));
    expect(v.ruleCapacity, toBe(0));
}

void test_validator_free_multiple_calls() {
    JsonValidator v = createValidator();
    addRuleSafe(&v, "field", "message");
    
    // Should be safe to call multiple times
    freeValidator(&v);
    freeValidator(&v);  // Should not crash
    
    expectNull(v.rules);
}

void test_validator_validate_null_body() {
    JsonValidator v = createValidator();
    requiredSafe(&v, "name");
    
    bool result = validate(&v, NULL);
    expect(result, toBe(0));  // false
    expectNotNull(v.error);
    
    freeValidator(&v);
}

void test_validator_validate_missing_field() {
    JsonValidator v = createValidator();
    requiredSafe(&v, "missing_field");
    
    // Create a JSON body without the required field
    JsonBuilder *body = jsonBuilder();
    jsonPutString(body, "other_field", "value");
    
    bool result = validate(&v, body);
    expect(result, toBe(0));  // false
    expectNotNull(v.error);
    
    freeValidator(&v);
    freeJsonBuilder(body);
}

void test_validator_validate_success() {
    JsonValidator v = createValidator();
    requiredSafe(&v, "name");
    
    // Create a JSON body with the required field
    JsonBuilder *body = jsonBuilder();
    jsonPutString(body, "name", "John");
    
    bool result = validate(&v, body);
    expect(result, toBe(1));  // true
    
    // Validator should be freed automatically on success
    // So we don't call freeValidator here
    
    freeJsonBuilder(body);
}

void test_validator_grow_capacity() {
    JsonValidator v = createValidator();
    
    // Add more rules than initial capacity
    for (int i = 0; i < 20; i++) {
        char field[32];
        snprintf(field, sizeof(field), "field%d", i);
        LavuError err = addRuleSafe(&v, field, "message");
        expect(err, toBe(LAVU_OK));
    }
    
    expect(v.ruleCount, toBe(20));
    expectNot(v.ruleCapacity, toBe(0));
    
    freeValidator(&v);
}

void runValidatorTests() {
    printf("\n--- Validator Tests ---\n");
    runTest(test_validator_create);
    runTest(test_validator_add_rule_safe);
    runTest(test_validator_add_rule_safe_null);
    runTest(test_validator_required_safe);
    runTest(test_validator_free_cleans_strings);
    runTest(test_validator_free_multiple_calls);
    runTest(test_validator_validate_null_body);
    runTest(test_validator_validate_missing_field);
    runTest(test_validator_validate_success);
    runTest(test_validator_grow_capacity);
}
