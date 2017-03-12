#define CATCH_CONFIG_MAIN

#include "spec/spec.h"

#include "src/exec.h" // exec

// Include new test files in the Makefile.
// Include global helpers here.

vm::Value resultOf(bytecode::CodeBlob instructions) {
  return (VM()).run(&instructions);
}

vm::Value resultOf(std::string code) {
  return execCode(code);
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
