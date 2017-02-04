#define CATCH_CONFIG_MAIN
#include "vendor/catch.h"

// #include "src/errors.h"
// #include "src/parser.h"
// #include "src/interpreter/machine.h"

// Include new test files in the Makefile.
// Include global helpers here.

// Get the value of a code listing.
// std::string resultOf(const char code[]) {
//   // Useful for debugging!
//   // std::cout << code << std::endl;

//   astBlock* program = (new fnParser())->parseCode(code);
//   fnMachine* context = new fnMachine();
//   fnValue* value = program->execute(context);

//   return value->asString();
// }

// bool failure(const char code[]) {
//   bool failed = false;

//   try {
//     astBlock* program = (new fnParser())->parseCode(code);
//     fnMachine* context = new fnMachine();
//     fnValue* value = program->execute(context);
//   } catch (FnError &e) {
//     failed = true;
//   }

//   return failed;
// }
