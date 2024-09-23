#pragma once

#include <stdbool.h>
#include <stdint.h>

// #include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_NATIVE(value) \
    (((ObjNative*)AS_OBJ(value))->function)
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
    OBJ_NATIVE,
    OBJ_STRING
} ObjType;

typedef struct {
    ObjType type;
} Obj;

typedef Value (*NativeFn)(int argc, Value* argv);

typedef struct {
    Obj obj;
    NativeFn function;
} ObjNative;

typedef struct {
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
} ObjString;

ObjNative* newNative(NativeFn function);
ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);
void printObject(Value value);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}
