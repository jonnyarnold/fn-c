// The Fn Code Generator converts an AST into a set of instructions.

#pragma once

#include "src/ast.h"
#include "src/bytecode.h"

namespace fn {

  // A Generator is used to generate instructions
  // from a Fn AST.
  class CodeGenerator {

    // Enables debug messages.
    bool debug;

  public:
    bytecode::CodeBlob instructions;

    CodeGenerator(bool debug) {
      this->debug = debug;
      this->instructions = bytecode::CodeBlob();
    }
    CodeGenerator() : CodeGenerator(false) {}

    // Add the instructions for the given statement
    // into the existing instructions.
    bytecode::CodeBlob digestTopLevel(ast::Statement* statement);

    bytecode::CodeBlob digest(ast::Statement* statement);
    bytecode::CodeBlob digest(ast::Id* id);
    bytecode::CodeBlob digest(ast::Deref* deref);
    bytecode::CodeBlob digest(ast::Assignment* assignment);
    bytecode::CodeBlob digest(ast::Block* block);
    bytecode::CodeBlob digest(ast::BlockValue* block);
    bytecode::CodeBlob digest(ast::Bool* b);
    bytecode::CodeBlob digest(ast::Number* n);
    bytecode::CodeBlob digest(ast::String* s);
    bytecode::CodeBlob digest(ast::Call* call);
    bytecode::CodeBlob digest(ast::Def* def);
    bytecode::CodeBlob digest(ast::Condition* condition);
    bytecode::CodeBlob digest(ast::Conditional* conditional);

  };

}
