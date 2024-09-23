#pragma once

typedef enum {
    TOKEN_COLON, // Definitions
    TOKEN_DOT, // Accessing fields of a struct
    TOKEN_EOF, // End of file
    TOKEN_ERROR, // Scanning issue
    TOKEN_IDENTIFIER, // Reference
    TOKEN_NUMBER, // Numeric literal
    TOKEN_PLUS, // Add
    TOKEN_STRING, // String literal
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

void initScanner(const char* source);
Token scanToken();
