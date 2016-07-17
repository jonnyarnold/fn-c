#pragma once

#include "src/interpreter/objects/value.h"
#include "src/interpreter/objects/bool.h"
#include "src/interpreter/objects/def.h"
#include "src/interpreter/machine.h"

// Object equality comparison.
class fnOr : public bareFnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    bool result =
      this->parent->asBool() ||
      values[0]->asBool();

    return new fnBool(result);
  }

public:
  fnOr(fnValue* parent) : bareFnDef(
    parent, 
    new std::vector<std::string>{"other"}
  ) {};
};
