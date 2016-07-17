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
protected:
  // The names of parameters of this definition.
  Strings* params;

  // Bare function definition.
  // See fnValue for more details.
  fnDef() : fnValue() {}

public:
  fnDef(fnValue* parent, Strings* params) : fnValue(parent) {
    this->params = params;
  }

  // Call is used by the machine
  // to set up the arguments and 
  // then pass to execute().
  fnValue* call(fnMachine*, std::vector<fnValue*>) override;

  // The operation encapsulated by this object.
  virtual fnValue* execute(fnMachine*, std::vector<fnValue*>) = 0;

  // Functions are only equal by reference.
  virtual std::size_t hash() override {
    return (std::size_t)(this);
  }

  virtual std::string asString(int indentationLevel) override;
};

// Some functions need to be bare (do not have locals)
// to avoid an infinite loop on initialization.
// Check out fnValue for more info.
class bareFnDef : public fnDef {
protected:
  bareFnDef(fnValue* parent, Strings* params) : fnDef() {
    this->parent = parent;
    this->params = params;
  }
};

// Denotes a fnDef whose implementation is a fn block.
class fnFnDef : public fnDef {
  // The block of instructions to execute when called.
  astBlock* block;

public:
  fnFnDef(astBlock* block, fnValue* parent, Strings* params) : fnDef(parent, params) { 
    this->block = block;
  }

  virtual fnValue* execute(fnMachine* context, std::vector<fnValue*> values) override {
    return this->block->execute(context);
  }

  virtual std::string asString(int indentationLevel) override;
};
