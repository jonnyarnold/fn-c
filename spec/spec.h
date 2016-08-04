#ifndef HELPERS
#define HELPERS

#include "vendor/catch.h" // Needed by all specs.
#include "src/errors.h"

// Get the value of a code listing.
std::string resultOf(const char code[]);

// Expect a code listing to fail.
bool failure(const char code[]);

#endif
