#pragma once

#include "src/interpreter/objects/value.h"
#include "src/interpreter/objects/bool.h"
#include "src/interpreter/objects/def.h"
#include "src/interpreter/machine.h"

// Object equality comparison.
class fnEq : public bareFnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    bool result = this->parent->hash() == values[0]->hash();
    return new fnBool(result);
  }

public:
  fnEq(fnValue* parent) : bareFnDef(
    parent, 
    new std::vector<std::string>{"other"}
  ) {};
};
