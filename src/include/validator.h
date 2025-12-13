#ifndef validator_h
#define validator_h

#include "lavu_error.h"
#include "json.h"

typedef struct {
    char *field;
    char *message;
} JsonValidationRule;

typedef struct {
    JsonValidationRule *rules;
    int                 ruleCount;
    int                 ruleCapacity;

    const char         *error;  // Points to rule message or static string
} JsonValidator;

// Create a new validator (returns validator with NULL rules on allocation failure)
JsonValidator createValidator(void);

// Free all memory associated with validator
// Safe to call multiple times
void freeValidator(JsonValidator *validator);

// Add a validation rule (safe version returns error code)
LavuError addRuleSafe(JsonValidator *v, const char *field, const char *message);

// Add a validation rule (exits on failure - legacy API)
void addRule(JsonValidator *v, const char *field, const char *message);

// Validate JSON body against all rules
// Returns true if valid, false otherwise (check v->error for message)
// On success, automatically frees validator
// On failure, caller must call freeValidator after handling error
bool validate(JsonValidator *v, JsonBuilder *body);

// Add required field rule (safe version returns error code)
LavuError requiredSafe(JsonValidator *v, const char *field);

// Add required field rule (exits on failure - legacy API)
void required(JsonValidator *v, const char *field);

// Check if field exists in JSON body
bool validateRequired(JsonBuilder *builder, char *field);

#endif