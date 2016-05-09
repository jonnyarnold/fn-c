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

// The top-level block.
class fnTopBlock : public fnBlock {

public:
  fnTopBlock() : fnBlock(NULL) {
    ValueDict* values =  new ValueDict{
      {"+", new fnDef(&add, this, new Strings{"first", "second"})}
    };

    this->locals = (*values);
  }
};

#endif
