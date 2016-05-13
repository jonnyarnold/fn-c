#ifndef FN_BUILTINS
#define FN_BUILTINS

#include "src/interpreter/runtime.h"

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

fnValue* fnAnd(fnExecution* context, std::vector<fnValue*> values) {
  std::cout << "AND(first " << values[0] << ", second " << values[1] << ")\n";
  bool result = 
    static_cast<fnBool*>(values[0])->value && 
    static_cast<fnBool*>(values[1])->value;

  return new fnBool(result);
}

fnValue* fnOr(fnExecution* context, std::vector<fnValue*> values) {
  std::cout << "OR(first " << values[0] << ", second " << values[1] << ")\n";
  bool result = 
    static_cast<fnBool*>(values[0])->value || 
    static_cast<fnBool*>(values[1])->value;

  return new fnBool(result);
}

fnValue* fnNot(fnExecution* context, std::vector<fnValue*> values) {
  std::cout << "NOT(" << values[0] << ")\n";
  bool result = !(static_cast<fnBool*>(values[0])->value);

  return new fnBool(result);
}

fnValue* fnEq(fnExecution* context, std::vector<fnValue*> values) {
  std::cout << "EQ(first " << values[0] << ", second " << values[1] << ")\n";
  bool result = values[0]->hash() == values[1]->hash();

  return new fnBool(result);
}

fnValue* print(fnExecution* context, std::vector<fnValue*> values) {
  std::cout << "PRINT(" << values[0] << ")\n";
  std::cout << values[0]->asString();


  return new fnString(new std::string(values[0]->asString()));
}


// The top-level block.
class fnTopBlock : public fnBlock {

public:
  fnTopBlock() : fnBlock(NULL) {
    ValueDict* values =  new ValueDict{
      {"+", new fnDef(&add, this, new Strings{"first", "second"})},
      {"-", new fnDef(&subtract, this, new Strings{"first", "second"})},
      {"*", new fnDef(&multiply, this, new Strings{"first", "second"})},
      {"/", new fnDef(&divide, this, new Strings{"first", "second"})},
      {"and", new fnDef(&fnAnd, this, new Strings{"first", "second"})},
      {"or", new fnDef(&fnOr, this, new Strings{"first", "second"})},
      {"not", new fnDef(&fnNot, this, new Strings{"first"})},
      {"eq", new fnDef(&fnNot, this, new Strings{"first", "second"})},
      {"print", new fnDef(&print, this, new Strings{"first"})},
    };

    this->locals = (*values);
  }
};

#endif
