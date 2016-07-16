#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "vendor/catch.h"

#include "src/parser.h"
#include "src/interpreter/machine.h"

// Include new test files in the Makefile.
// Include global helpers here.

// Get the value of a code listing.
std::string resultOf(const char code[]) {
  astBlock* program = (new fnParser())->parseCode(code);
  fnMachine* context = new fnMachine();
  fnValue* value = program->execute(context);

  return value->asString();
}
