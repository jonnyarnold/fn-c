#define CATCH_CONFIG_MAIN

#include "spec/spec.h"

// Include new test files in the Makefile.
// Include global helpers here.

vm::Value resultOf(bytecode::CodeBlob instructions) {
  return (VM()).run(&instructions);
}

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
