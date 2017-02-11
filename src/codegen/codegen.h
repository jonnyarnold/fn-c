// The Fn Code Generator converts an AST into a set of instructions.

#pragma once

#include <list>

#include "src/ast.h"
#include "src/bytecode.h"

namespace fn {

  // A Generator is used to generate instructions
  // from a Fn AST.
  class CodeGenerator {
    bool debug;

  public:
    bytecode::CodeBlob* instructions;
    
    CodeGenerator(bool debug) { 
      this->debug = debug;
    }
    CodeGenerator() : CodeGenerator(false) {}

    // Add the instructions for the given statement
    // into the existing instructions.
    void digest(ast::Id* id);
    void digest(ast::Deref* deref);
    void digest(ast::Assignment* assignment);
    void digest(ast::Block* block);
    void digest(ast::Bool* b);
    void digest(ast::Number* n);
    void digest(ast::String* s);
    void digest(ast::Call* call);
    void digest(ast::Def* def);
    void digest(ast::Condition* condition);
    void digest(ast::Conditional* conditional);


  };

}
