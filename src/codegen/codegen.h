// The Fn Code Generator converts an AST into a set of instructions.

#pragma once

#include "src/ast.h"
#include "src/bytecode.h"

namespace fn {

  // A Generator is used to generate instructions
  // from a Fn AST.
  class CodeGenerator {
    bool debug;

  public:
    bytecode::CodeByte* instructions;
    unsigned int instructionByteCount;

    CodeGenerator(bool debug) { this->debug = debug; }
    CodeGenerator() : CodeGenerator(false) {}

    // Return the instructions for the given statement.
    void digest(ast::Statement* code);
  };

}
