#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "types.h"

typedef enum
{
    VALUETYPE_BOOL,
    VALUETYPE_WORD,
    VALUETYPE_FILE,
    VALUETYPE_I64,
    VALUETYPE_CHARS,
} ValueType;

typedef union
{
    bool asBool;
    Word asWord;
    FILE *asFile;
    I64 asI64;
    // TODO: Replace char* with our own length-aware string type
    char *asChars;
} ValueData;

typedef struct
{
    ValueType type;
    ValueData data;
} Value;

#define VALUE_WRAP_BOOL(w) ((Value){VALUETYPE_BOOL, {.asBool = (w)}})
#define VALUE_IS_BOOL(v) (v.type == VALUETYPE_BOOL)
#define VALUE_UNWRAP_BOOL(v) (v.data.asBool)

#define VALUE_WRAP_WORD(w) ((Value){VALUETYPE_WORD, {.asWord = (w)}})
#define VALUE_IS_WORD(v) (v.type == VALUETYPE_WORD)
#define VALUE_UNWRAP_WORD(v) (v.data.asWord)
Value Word_bitAt(Value word, Value index);

#define VALUE_WRAP_FILE(file) ((Value){VALUETYPE_FILE, {.asFile = file}})
#define VALUE_IS_FILE(v) (v.type == VALUETYPE_FILE)
#define VALUE_UNWRAP_FILE(v) (v.data.asFile)
Value File_open(Value filename, Value mode);
Value File_read(Value file);
Value File_write(Value file, Value data);
Value File_close(Value file);

#define VALUE_WRAP_I64(i) ((Value){VALUETYPE_I64, {.asI64 = (i)}})
#define VALUE_IS_I64(v) (v.type == VALUETYPE_I64)
#define VALUE_UNWRAP_I64(v) (v.data.asI64)

#define VALUE_WRAP_CHARS(i) ((Value){VALUETYPE_CHARS, {.asChars = (i)}})
#define VALUE_IS_CHARS(v) (v.type == VALUETYPE_CHARS)
#define VALUE_UNWRAP_CHARS(v) (v.data.asChars)
