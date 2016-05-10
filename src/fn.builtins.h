#ifndef FN_BUILTINS
#define FN_BUILTINS

#include "fn.runtime.h"

// Adds two integers together.
fnValue* add(std::vector<fnValue*> values) {
  int sum = 
    static_cast<fnInt*>(values[0])->value + 
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(sum);
}

// Subtracts two integers.
fnValue* subtract(std::vector<fnValue*> values) {
  int diff = 
    static_cast<fnInt*>(values[0])->value - 
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(diff);
}

// Multiplies two integers.
fnValue* multiply(std::vector<fnValue*> values) {
  int product = 
    static_cast<fnInt*>(values[0])->value * 
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(product);
}

// Divides two integers.
fnValue* divide(std::vector<fnValue*> values) {
  int quotient = 
    static_cast<fnInt*>(values[0])->value / 
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(quotient);
}

// The top-level block.
class fnTopBlock : public fnBlock {

public:
  fnTopBlock() : fnBlock(NULL) {
    ValueDict* values =  new ValueDict{
      {"+", new fnDef(&add, this, new Strings{"first", "second"})},
      {"-", new fnDef(&subtract, this, new Strings{"first", "second"})},
      {"*", new fnDef(&multiply, this, new Strings{"first", "second"})},
      {"/", new fnDef(&divide, this, new Strings{"first", "second"})}
    };

    this->locals = (*values);
  }
};

#endif
