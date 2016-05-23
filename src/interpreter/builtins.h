#pragma once

#include "src/interpreter/runtime.h"

fnValue* add(fnMachine* context, std::vector<fnValue*> values);
fnValue* subtract(fnMachine* context, std::vector<fnValue*> values);
fnValue* multiply(fnMachine* context, std::vector<fnValue*> values);
fnValue* divide(fnMachine* context, std::vector<fnValue*> values);
fnValue* fnAnd(fnMachine* context, std::vector<fnValue*> values);
fnValue* fnOr(fnMachine* context, std::vector<fnValue*> values);
fnValue* fnNot(fnMachine* context, std::vector<fnValue*> values);
fnValue* fnEq(fnMachine* context, std::vector<fnValue*> values);
fnValue* print(fnMachine* context, std::vector<fnValue*> values);
fnValue* list(fnMachine* context, std::vector<fnValue*> values);

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
      {"eq", new fnDef(&fnEq, this, new Strings{"first", "second"})},
      {"print", new fnDef(&print, this, new Strings{"first"})},
      {"List", new fnDef(&list, this, new Strings{"...args"})},
    };

    this->locals = (*values);
  }
};
