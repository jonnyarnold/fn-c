#ifndef FN_BUILTINS
#define FN_BUILTINS

#include "interpreter/runtime.h"

// Adds two integers together.
fnValue* add(fnExecution* context, std::vector<fnValue*> values) {
  std::cout << "ADD(first " << values[0] << ", second " << values[1] << ")\n";
  int sum = 
    static_cast<fnInt*>(values[0])->value + 
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(sum);
}

// Subtracts two integers.
fnValue* subtract(fnExecution* context, std::vector<fnValue*> values) {
  std::cout << "SUBTRACT(first " << values[0] << ", second " << values[1] << ")\n";
  int diff = 
    static_cast<fnInt*>(values[0])->value - 
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(diff);
}

// Multiplies two integers.
fnValue* multiply(fnExecution* context, std::vector<fnValue*> values) {
  std::cout << "MULTIPLY(first " << values[0] << ", second " << values[1] << ")\n";
  int product = 
    static_cast<fnInt*>(values[0])->value * 
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(product);
}

// Divides two integers.
fnValue* divide(fnExecution* context, std::vector<fnValue*> values) {
  std::cout << "DIVIDE(first " << values[0] << ", second " << values[1] << ")\n";
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
