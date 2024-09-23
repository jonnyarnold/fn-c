#pragma once

#include <string.h>

typedef enum {
    // Treat the value as either True or False.
    V_BOOL
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
    } as;
} Value;

bool Value_isBool(Value v) { return v.type == V_BOOL; }
Value Value_fromBool(bool b) { return (Value) { V_BOOL, { .boolean = b } }; }
bool Value_toBool(Value v) { return v.as.boolean; }

Value Bool_not(Value v) { return Value_fromBool(!Value_toBool(v)); }
