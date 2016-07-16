#pragma once

#include <string>
#include <vector>
#include <functional>
#include <iostream>

#include "src/interpreter/objects/value.h"
#include "src/ast.h"

// Forward declaration.
class fnMachine;

// A vector of strings.
typedef std::vector<std::string> Strings;

// The style of function used by Fn.
typedef std::function<fnValue*(fnMachine*, std::vector<fnValue*>)> fnFunc;

// Represents a function definition.
class fnDef : public fnValue {
  
  // The parent value of this definition.
  // This can be accessed in-function using context->currentScope()->parent;
  fnValue* parentScope;

  // The names of parameters of this definition.
  Strings* params;

  // The function that will be executed when called.
  fnFunc func;

public:
  fnDef(fnFunc instructions, fnValue* parentScope, Strings* params) {
    this->parentScope = parentScope;
    this->params = params;
    this->func = instructions;
  }

  fnDef(astBlock* block, fnValue* parentScope, Strings* params) {
    this->parentScope = parentScope;
    this->params = params;
    this->func = [block](fnMachine* context, std::vector<fnValue*> values) { return block->execute(context); };
  }

  fnValue* call(fnMachine*, std::vector<fnValue*>) override;

  // Functions are only equal by reference.
  virtual std::size_t hash() override {
    return (std::size_t)(this);
  }

  virtual std::string asString(int indentationLevel) override;
};
