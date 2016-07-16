#pragma once

#include "src/interpreter/objects/value.h"
#include "src/interpreter/objects/def.h"

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
fnValue* dump(fnMachine* context, std::vector<fnValue*> values);

// The top-level block.
class fnWorld : public fnValue {

public:
  fnWorld() : fnValue() {

    // TODO: Move most of these to their respective objects.
    ValueDict* values =  new ValueDict{
      {"+", new fnDef(&add, this, new Strings{"first", "second"})}, // move to fnNumber
      {"-", new fnDef(&subtract, this, new Strings{"first", "second"})}, // move to fnNumber
      {"*", new fnDef(&multiply, this, new Strings{"first", "second"})}, // move to fnNumber
      {"/", new fnDef(&divide, this, new Strings{"first", "second"})}, // move to fnNumber
      {"and", new fnDef(&fnAnd, this, new Strings{"first", "second"})}, // move to fnValue
      {"or", new fnDef(&fnOr, this, new Strings{"first", "second"})}, // move to fnValue
      {"not", new fnDef(&fnNot, this, new Strings{"first"})}, // move to fnValue
      {"eq", new fnDef(&fnEq, this, new Strings{"first", "second"})}, // move to fnValue
      {"print", new fnDef(&print, this, new Strings{"out"})},
      {"dump", new fnDef(&dump, this, new Strings{})},
      {"List", new fnDef(&list, this, new Strings{"...args"})},
    };

    this->locals = (*values);
  }
};
