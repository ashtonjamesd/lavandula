#include "include/middleware.h"
#include "include/api_response.h"
#include "include/validator.h"
#include "include/lavu_error.h"

// Initial capacity for validator rules
#define VALIDATOR_INITIAL_CAPACITY 4

JsonValidator createValidator() {
    JsonValidator v = {
        .rules = malloc(sizeof(JsonValidationRule) * VALIDATOR_INITIAL_CAPACITY),
        .ruleCount = 0,
        .ruleCapacity = VALIDATOR_INITIAL_CAPACITY,
        .error = NULL
    };
    
    // Handle allocation failure gracefully
    if (!v.rules) {
        v.ruleCapacity = 0;
    }
    
    return v;
}

void freeValidator(JsonValidator *validator) {
    if (!validator) return;
    
    // Free all strdup's strings in rules
    for (int i = 0; i < validator->ruleCount; i++) {
        free(validator->rules[i].field);
        free(validator->rules[i].message);
    }
    
    free(validator->rules);
    validator->rules = NULL;
    validator->ruleCount = 0;
    validator->ruleCapacity = 0;
    validator->error = NULL;
}

LavuError addRuleSafe(JsonValidator *v, const char *field, const char *message) {
    if (!v || !field || !message) {
        return LAVU_ERR_NULL_PTR;
    }
    
    // Check if we need to grow
    if (v->ruleCount >= v->ruleCapacity) {
        int newCapacity = v->ruleCapacity == 0 ? VALIDATOR_INITIAL_CAPACITY : v->ruleCapacity * 2;
        JsonValidationRule *newRules = realloc(v->rules, sizeof(JsonValidationRule) * newCapacity);
        if (!newRules) {
            return LAVU_ERR_NOMEM;
        }
        v->rules = newRules;
        v->ruleCapacity = newCapacity;
    }

    char *fieldCopy = strdup(field);
    if (!fieldCopy) {
        return LAVU_ERR_NOMEM;
    }
    
    char *messageCopy = strdup(message);
    if (!messageCopy) {
        free(fieldCopy);
        return LAVU_ERR_NOMEM;
    }

    v->rules[v->ruleCount].field = fieldCopy;
    v->rules[v->ruleCount].message = messageCopy;
    v->ruleCount++;
    
    return LAVU_OK;
}

void addRule(JsonValidator *v, const char *field, const char *message) {
    LavuError err = addRuleSafe(v, field, message);
    if (lavuIsError(err)) {
        fprintf(stderr, "Fatal: %s\n", lavuErrorStr(err));
        exit(EXIT_FAILURE);
    }
}

bool validateRequired(JsonBuilder *builder, char *field) {
    return jsonHasKey(builder, field);
}

LavuError requiredSafe(JsonValidator *v, const char *field) {
    char message[256];
    snprintf(message, sizeof(message), "The field '%s' is required.", field);
    return addRuleSafe(v, field, message);
}

void required(JsonValidator *v, const char *field) {
    LavuError err = requiredSafe(v, field);
    if (lavuIsError(err)) {
        fprintf(stderr, "Fatal: %s\n", lavuErrorStr(err));
        exit(EXIT_FAILURE);
    }
}

bool validate(JsonValidator *v, JsonBuilder *body) {
    if (!v) {
        return false;
    }
    
    if (!body) {
        v->error = "Request body is missing or malformed.";
        return false;
    }

    for (int i = 0; i < v->ruleCount; i++) {
        JsonValidationRule *rule = &v->rules[i];

        if (!validateRequired(body, rule->field)) {
            v->error = rule->message;
            // Don't free here - caller should call freeValidator after handling error
            return false;
        }
    }

    // Validation passed - clean up now
    freeValidator(v);

    return true;
}