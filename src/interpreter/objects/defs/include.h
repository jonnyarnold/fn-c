#pragma once

#include "src/interpreter/objects/value.h"
#include "src/interpreter/objects/bool.h"
#include "src/interpreter/objects/def.h"
#include "src/interpreter/machine.h"

// Includes the contents of a value
// into the current value.
//
// As with traditional assignment,
// if a value has already been defined,
// an error will occur.
class fnInclude : public bareFnDef {
  fnValue* execute(fnMachine* context, std::vector<fnValue*> values) {
    // Look at each of the locals in the given block
    // and import them into the parent.
    for (auto local : values[0]->locals) {
      // TODO: Fix this violation of type safety.
      std::string* key = const_cast<std::string*>(&local.first);
      fnValue* value = local.second;
      
      this->parent->set(key, value);
    }
  }

public:
  fnInclude(fnValue* parent) : bareFnDef(
    parent, 
    new std::vector<std::string>{"value"}
  ) {};
};
