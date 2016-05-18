#ifndef FN_BUILTINS
#define FN_BUILTINS

#include "src/interpreter/runtime.h"

// Adds two integers together.
fnValue* add(fnMachine* context, std::vector<fnValue*> values) {
  int sum =
    static_cast<fnInt*>(values[0])->value +
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(sum);
}

// Subtracts two integers.
fnValue* subtract(fnMachine* context, std::vector<fnValue*> values) {
  int diff =
    static_cast<fnInt*>(values[0])->value -
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(diff);
}

// Multiplies two integers.
fnValue* multiply(fnMachine* context, std::vector<fnValue*> values) {
  int product =
    static_cast<fnInt*>(values[0])->value *
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(product);
}

// Divides two integers.
fnValue* divide(fnMachine* context, std::vector<fnValue*> values) {
  int quotient =
    static_cast<fnInt*>(values[0])->value /
    static_cast<fnInt*>(values[1])->value;

  return new fnInt(quotient);
}

fnValue* fnAnd(fnMachine* context, std::vector<fnValue*> values) {
  bool result =
    static_cast<fnBool*>(values[0])->value &&
    static_cast<fnBool*>(values[1])->value;

  return new fnBool(result);
}

fnValue* fnOr(fnMachine* context, std::vector<fnValue*> values) {
  bool result =
    static_cast<fnBool*>(values[0])->value ||
    static_cast<fnBool*>(values[1])->value;

  return new fnBool(result);
}

fnValue* fnNot(fnMachine* context, std::vector<fnValue*> values) {
  bool result = !(static_cast<fnBool*>(values[0])->value);

  return new fnBool(result);
}

fnValue* fnEq(fnMachine* context, std::vector<fnValue*> values) {
  bool result = values[0]->hash() == values[1]->hash();

  return new fnBool(result);
}

fnValue* print(fnMachine* context, std::vector<fnValue*> values) {
  std::cout << values[0]->asString() << "\n";
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
