// The Fn Code Generator converts an AST into a set of instructions.

#pragma once

#include <unordered_map>
#include <stack>

#include "src/ast.h"
#include "src/bytecode.h"

namespace fn {

  typedef std::unordered_map<std::string, bytecode::ValueIndex> ValueIndexMap;

  class ValueIndexTable {
  private:
    ValueIndexMap valueIndexMap;
    bytecode::ValueIndex _lastIndex;

  public:
    ValueIndexTable() {
      this->valueIndexMap = ValueIndexMap();
      this->_lastIndex = 0;
    }

    bytecode::ValueIndex get(std::string name) {
      return this->valueIndexMap[name];
    }

    void set(std::string name, bytecode::ValueIndex index) {
      this->valueIndexMap[name] = index;
    }

    void set(std::string name) {
      this->_lastIndex += 1;
      this->valueIndexMap[name] = this->_lastIndex;
    }

    void advanceIndex() {
      this->_lastIndex += 1;
    }

    bytecode::ValueIndex lastIndex() {
      return this->_lastIndex;
    }
  };

  // A Generator is used to generate instructions
  // from a Fn AST.
  class CodeGenerator {

    // Enables debug messages.
    bool debug;

    // fn bytecode only has indices for values, and no names.
    // We need to keep track of the index for each named variable.
    // We need a new table for each block.
    std::stack<ValueIndexTable> valueIndexStack;

  public:
    bytecode::CodeBlob instructions;

    CodeGenerator(bool debug) {
      this->debug = debug;
      this->instructions = bytecode::CodeBlob();
      this->valueIndexStack = std::stack<ValueIndexTable>();
      this->valueIndexStack.push(ValueIndexTable());
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
    bytecode::CodeBlob digest(ast::Bool* b);
    bytecode::CodeBlob digest(ast::Number* n);
    bytecode::CodeBlob digest(ast::String* s);
    bytecode::CodeBlob digest(ast::Call* call);
    bytecode::CodeBlob digest(ast::Def* def);
    bytecode::CodeBlob digest(ast::Condition* condition);
    bytecode::CodeBlob digest(ast::Conditional* conditional);

    bytecode::ValueIndex rememberIndexFor(ast::Reference* reference);
    bytecode::ValueIndex getIndexFor(ast::Reference* reference);
    bytecode::ValueIndex rememberIndexFor(std::string name);
    bytecode::ValueIndex getIndexFor(std::string name);

    ValueIndexTable* currentTable();
  };

}
