#include "scanner.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static char peek() {
    return *scanner.current;
}

static char peekNext() {
    if (isAtEnd())
        return '\0';
    return scanner.current[1];
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    return token;
}

static void skipWhitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
        case ' ':
        case '\r':
            advance();
            break;
        case '\n':
            scanner.line++;
            advance();
            break;

        case '#':
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd())
                advance();
            break;

        default:
            return;
        }
    }
}

static Token identifier() {
    while (isAlpha(peek()) || isDigit(peek()))
        advance();
    return makeToken(TOKEN_IDENTIFIER);
}

static Token number() {
    while (isDigit(peek()))
        advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the ".".
        advance();

        while (isDigit(peek()))
            advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n')
            scanner.line++;
        advance();
    }

    if (isAtEnd())
        return errorToken("Unterminated string.");

    // The closing quote.
    advance();
    return makeToken(TOKEN_STRING);
}

Token scanToken() {
    skipWhitespace();

    scanner.start = scanner.current;

    if (isAtEnd())
        return makeToken(TOKEN_EOF);

    char c = advance();
    if (isAlpha(c))
        return identifier();

    if (isDigit(c))
        return number();

    switch (c) {
    case '.':
        return makeToken(TOKEN_DOT);
    case '+':
        return makeToken(TOKEN_PLUS);
    case '"':
        return string();
    }

    return errorToken("Unexpected character.");
}
