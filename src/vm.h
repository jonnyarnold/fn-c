#pragma once

#include "types.h"

typedef enum
{
    VALUETYPE_BOOL,
    VALUETYPE_WORD,
    VALUETYPE_FILE,
    VALUETYPE_I64,
    // TODO: VALUETYPE_POINTER to point to heap objects
} ValueType;

// Values fit into registers
typedef struct
{
    ValueType type;
    union
    {
        bool asBool;
        Word asWord;
        int asFile;
        I64 asI64;
    } data;
} Value;

#define VALUE_WRAP_BOOL(w) ((Value){VALUETYPE_BOOL, {.asBool = (w)}})
#define VALUE_IS_BOOL(v) (v.type == VALUETYPE_BOOL)
#define VALUE_UNWRAP_BOOL(v) (v.data.asBool)

#define VALUE_WRAP_WORD(w) ((Value){VALUETYPE_WORD, {.asWord = (w)}})
#define VALUE_IS_WORD(v) (v.type == VALUETYPE_WORD)
#define VALUE_UNWRAP_WORD(v) (v.data.asWord)
Value Word_bitAt(Value word, Value index);

// #define VALUE_WRAP_FILE(file) ((Value){VALUETYPE_FILE, {.file = file}})
// #define VALUE_IS_FILE(v) (v.type == VALUETYPE_FILE)
// #define VALUE_UNWRAP_FILE(v) (v.data.file)
// Value File_open(Value filename, Value mode);
// Value File_read(Value file);
// Value File_write(Value file, Value data);
// Value File_close(Value file);

#define VALUE_WRAP_I64(i) ((Value){VALUETYPE_I64, {.asI64 = (i)}})
#define VALUE_IS_I64(v) (v.type == VALUETYPE_I64)
#define VALUE_UNWRAP_I64(v) (v.data.asI64)
